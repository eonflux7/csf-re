// Differential testing, tier 3: record/replay of a diffed function's callees (tools/difftable.py, replay_table.inc).
//
// Every callee of a tier-3 function gets an interceptor at its entry: whatever was there (our hook's jmp, a diff
// stub, or the original code through a MinHook trampoline) stays the pass-through target. Off the diff thread, or
// while no tier-3 diff runs, the interceptor just jumps on.
//
// hook/diff.cpp runs a tier-3 function F as: snapshot F's regions (this + written globals); RECORD: the original
// runs and each call it makes at depth 0 really executes, with the callee, ecx/edx, the stack args, the return
// value (eax, edx, st0) and the bytes it changed in F's regions and in stack buffers passed by pointer recorded
// (the callee returns through t3_post_stub, so every register reaches it exactly as the caller set it); restore
// the snapshot; REPLAY: ours runs, each call it makes is matched against the next record (callee, ecx/edx, args;
// pointers into the stack count as equal) and answered from it without executing (results returned, recorded
// bytes re-applied): each callee's side effects happen once. Compared afterwards: return value and regions as in
// tier 1, plus the call trace (here).
//
// When ours leaves the recording (another callee, one call too many, a recorded return it can't be given), its
// remaining calls could only run for real, doing their side effects a second time (v1 did: the first smoke died in
// a tree the callees had already changed). Instead ours is abandoned there: t3_pre longjmps back into diff.cpp's
// run(), which puts the original's memory back and returns the original's result.
//
// Callees' writes outside the snapshot regions and the caller's stack (heap objects) stay done when ours runs: ours
// may read them and take another path (0x6BF280 tests frame->parent, which the original's RwFrameRemoveChild had
// just cleared). Undoing them is unsafe (a freeing callee's bytes are the allocator's), so they're only watched,
// 256 bytes behind every heap pointer arg / ecx: a mismatch after such a write is inconclusive, not a DIFF.
#include <windows.h>
#include <intrin.h>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "MinHook.h"
#include "hook.h"
#include "diff_compare.h"

namespace {

using hook::diff_detail::on_stack;
using hook::diff_detail::same_arg;

struct Callee {
    uint32_t addr, pop, nargs, regs, ret;  // regs: 1 ecx, 2 edx; ret: 0 int32 1 fp 3 void 4 int64
    uint8_t widths[8];  // defined bytes per stack dword: 1 byte, 2 short, 4 full; 0/unknown also compares full
};
const Callee kCallees[] = {
#define CSF_T3_CALLEES
#include "replay_table.inc"
#undef CSF_T3_CALLEES
    {0, 0, 0, 0, 0, {}},
};
constexpr int kNCallees = sizeof kCallees / sizeof kCallees[0] - 1;

struct Regs { uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; };  // pushad layout

enum Mode { OFF, RECORD, REPLAY };

constexpr int kMaxRecs = 64, kMaxDeltas = 1024, kPeek = 1024;  // kPeek: bytes watched behind a stack pointer arg
struct Delta { uint32_t off; uint8_t where, val; };       // where: 0 F's regions, 1..8 stack arg k-1, 9 ecx
struct Rec {
    uint32_t callee, ecx, edx, args[8], eax, edxr;
    uint32_t pop;   // bytes the callee really popped: measured, a virtual reached by a tail jump pops its own
    double fp;
    int8_t retarg;  // eax was arg k (0..7) / ecx (8): ours gets its own; -1 a value; -2 an unmatched stack address
    uint32_t nd;
    bool overflow;
    bool heapw;     // changed bytes behind a heap pointer arg / ecx
    Delta d[kMaxDeltas];
};

Rec g_recs[kMaxRecs];
int g_nrec, g_cursor;
bool g_lost, g_heapw;  // a record didn't fit (calls or deltas): the comparison is inconclusive
Mode g_mode;
DWORD g_thread;
int g_depth;
uint32_t g_realret, g_retslot;  // the recorded call's return address and where it sat
uint8_t g_pre[16384], g_spre[10][kPeek];
uint32_t g_sptr[10];  // stack pointers watched during the current recorded call (0 = none)
uint32_t g_slen[10];  // how far each may reach: up to the recorded function's saved ebp (see t3_pre)
constexpr uint32_t kHeapPeek = 256;
uint8_t g_hpre[10][kHeapPeek];
uint32_t g_hptr[10];  // heap pointers watched (0 = none)
char g_msg[256];
bool g_mismatch;
int g_abort;      // ours was abandoned: 1 it diverged, 2 the recording can't answer it
void* g_jb[5];    // __builtin_setjmp buffer, armed by diff.cpp's run() around ours

struct Region { uint32_t addr, size; };
Region g_regions[9];
int g_nregions;

void* g_stubs;

bool in_region(uint32_t p);

// p .. p+n committed and readable (not a guard page)
bool readable(uint32_t p, uint32_t n) {
    if (p < 0x10000)
        return false;
    MEMORY_BASIC_INFORMATION mb;
    if (!VirtualQuery((void*)p, &mb, sizeof mb) || mb.State != MEM_COMMIT ||
        (mb.Protect & (PAGE_NOACCESS | PAGE_GUARD)) || !(mb.Protect & 0xEE))  // 0xEE: any readable protection
        return false;
    return p + n <= (uint32_t)mb.BaseAddress + mb.RegionSize;
}

uint32_t peek_len(uint32_t p) {
    uint32_t top = __readfsdword(4);
    return top - p < kPeek ? top - p : kPeek;
}

bool in_region(uint32_t p) {
    for (int k = 0; k < g_nregions; ++k)
        if (p - g_regions[k].addr < g_regions[k].size)
            return true;
    return false;
}

void region_copy(uint8_t* out) {
    for (int k = 0, o = 0; k < g_nregions; o += g_regions[k++].size)
        memcpy(out + o, (void*)g_regions[k].addr, g_regions[k].size);
}

void region_write(uint32_t off, uint8_t v) {
    for (int k = 0; k < g_nregions; off -= g_regions[k++].size)
        if (off < g_regions[k].size) {
            uint8_t* p = (uint8_t*)g_regions[k].addr + off;
            DWORD old;
            if (k && VirtualProtect(p, 1, PAGE_READWRITE, &old)) {  // globals (diff.cpp restore does the same)
                *p = v;
                VirtualProtect(p, 1, old, &old);
            } else {
                *p = v;
            }
            return;
        }
}

void mismatch(const char* fmt, ...) {
    if (g_mismatch)
        return;  // the first difference is the useful one
    g_mismatch = true;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_msg, sizeof g_msg, fmt, ap);
    va_end(ap);
}

// Stack args of a recorded call: the prototype's, or what the callee really popped if that's more
uint32_t nargs(const Callee& c, const Rec& R) {
    uint32_t n = c.nargs > R.pop / 4 ? c.nargs : R.pop / 4;
    return n < 8 ? n : 8;
}

[[noreturn]] void abandon(int why) {
    g_mode = OFF;
    g_abort = why;
    __builtin_longjmp(g_jb, 1);
}

void add_delta(Rec& R, uint8_t where, uint32_t off, uint8_t val) {
    if (R.nd >= kMaxDeltas) {
        R.overflow = g_lost = true;
        return;
    }
    R.d[R.nd++] = {off, where, val};
}

}  // namespace

extern "C" {
void t3_post_stub();
uint32_t g_t3_pop;      // bytes the replayed callee pops (read by t3_entry)
double g_t3_fp;         // st0 of a recorded / replayed fp return
uint32_t g_t3_postfp;   // the recorded callee returns in st0
void** g_t3_tp;         // pass-through targets, one per callee

// t3_entry: 0 = pass through to the callee, 1 = answered (eax/edx set in r), 2 = answered with st0 = g_t3_fp
int __cdecl t3_pre(Regs* r) {
    if (g_mode == OFF || g_depth || GetCurrentThreadId() != g_thread)
        return 0;
    uint32_t i = *(uint32_t*)((uint8_t*)r + 32);
    const Callee& c = kCallees[i];
    uint32_t* a = (uint32_t*)((uint8_t*)r + 40);
    if (g_mode == RECORD) {
        if (g_lost)
            return 0;  // ours won't run: nothing more to record
        if (g_nrec >= kMaxRecs) {
            g_lost = true;
            return 0;
        }
        Rec& R = g_recs[g_nrec++];
        R = Rec{};
        static const bool trace = hook::ini_string("diff_trace") && atoi(hook::ini_string("diff_trace"));
        if (trace)
            hook::log("T3REC call %d -> %08X ecx=%08X arg0=%08X", g_nrec, c.addr, r->ecx, a[0]);
        R.callee = i;
        R.ecx = r->ecx;
        R.edx = r->edx;
        uint32_t room = __readfsdword(4) - (uint32_t)a;  // all 8: the measured pop may show more args
        memcpy(R.args, a, room < sizeof R.args ? room : sizeof R.args);
        region_copy(g_pre);
        for (uint32_t k = 0; k < 10; ++k) {
            uint32_t p = k < 8 ? (k < c.nargs ? R.args[k] : 0) : k == 9 && (c.regs & 1) ? r->ecx : 0;
            g_sptr[k] = p && on_stack(p) && p > (uint32_t)a ? p : 0;  // a caller's local, not the callee's frame
            // A local of the recorded function ends below its frame pointer: [ebp] is the saved ebp, [ebp+4] the
            // return address. Without this cap a 1-byte out flag at [ebp-1] (0x759FC0, 0x7557C0) was replayed
            // widened to a dword, i.e. with 3 bytes of the original's saved ebp, into ours' frame: the /RTC stack
            // check then killed the game at startup (0xC0000409) once those initializers were in the table.
            g_slen[k] = g_sptr[k] ? peek_len(p) : 0;
            if (g_sptr[k] && r->ebp > p && r->ebp - p < g_slen[k])
                g_slen[k] = r->ebp - p;
            if (g_sptr[k])
                memcpy(g_spre[k], (void*)p, g_slen[k]);
            g_hptr[k] = p && !on_stack(p) && !in_region(p) && readable(p, kHeapPeek) ? p : 0;
            if (g_hptr[k])
                memcpy(g_hpre[k], (void*)p, kHeapPeek);
        }
        g_t3_postfp = c.ret == 1;
        g_retslot = (uint32_t)r + 36;
        g_realret = *(uint32_t*)g_retslot;
        *(uint32_t*)((uint8_t*)r + 36) = (uint32_t)(void*)&t3_post_stub;  // the callee returns to us first
        g_depth = 1;
        return 0;
    }
    // REPLAY. Diverged: running this call for real would repeat side effects the original's run already did.
    if (g_cursor >= g_nrec) {
        mismatch("call %d: ours calls %08X, the original made only %d calls", g_cursor + 1, c.addr, g_nrec);
        abandon(1);
    }
    Rec& R = g_recs[g_cursor];
    if (R.callee != i) {
        mismatch("call %d: ours calls %08X, the original %08X", g_cursor + 1, c.addr, kCallees[R.callee].addr);
        abandon(1);
    }
    if (R.retarg == -2)
        abandon(2);  // the original's stack address: dead in ours' frame layout
    ++g_cursor;
    if ((c.regs & 1) && !same_arg(R.ecx, r->ecx))
        mismatch("call %d to %08X: ecx orig %08X ours %08X", g_cursor, c.addr, R.ecx, r->ecx);
    if ((c.regs & 2) && !same_arg(R.edx, r->edx))
        mismatch("call %d to %08X: edx orig %08X ours %08X", g_cursor, c.addr, R.edx, r->edx);
    for (uint32_t k = 0, n = nargs(c, R); k < n; ++k)
        if (!same_arg(R.args[k], a[k], c.widths[k])) {
            mismatch("call %d to %08X: arg %u orig %08X ours %08X", g_cursor, c.addr, k + 1, R.args[k], a[k]);
            break;
        }
    for (uint32_t k = 0; k < R.nd; ++k) {  // the callee's writes, re-applied for ours
        const Delta& d = R.d[k];
        if (d.where == 0) {
            region_write(d.off, d.val);
        } else {
            uint32_t p = d.where == 9 ? r->ecx : a[d.where - 1];
            if (on_stack(p) && d.off < peek_len(p))
                *((uint8_t*)p + d.off) = d.val;
        }
    }
    r->eax = R.retarg == 8 ? r->ecx : R.retarg >= 0 ? a[R.retarg] : R.eax;
    r->edx = R.edxr;
    g_t3_pop = R.pop;
    if (c.ret == 1) {
        g_t3_fp = R.fp;
        return 2;
    }
    return 1;
}

// t3_post_stub: a recorded callee returned (eax/edx in r, st0 in g_t3_fp); returns the caller's return address
uint32_t __cdecl t3_post(Regs* r) {
    Rec& R = g_recs[g_nrec - 1];
    R.eax = r->eax;
    if (hook::ini_string("diff_trace") && atoi(hook::ini_string("diff_trace")))
        hook::log("T3REC ret from %08X eax=%08X", kCallees[R.callee].addr, r->eax);
    R.edxr = r->edx;
    R.fp = g_t3_postfp ? g_t3_fp : 0;
    R.pop = (uint32_t)r + 36 - (g_retslot + 4);  // esp before t3_post_stub's pushes, minus esp right after the call
    const Callee& c = kCallees[R.callee];
    R.retarg = -1;
    if (c.ret == 0 && R.eax) {  // returns one of its pointer args (vector ops return `out`, ctors `this`)
        if ((c.regs & 1) && R.eax == R.ecx)
            R.retarg = 8;
        for (uint32_t k = 0, n = nargs(c, R); R.retarg < 0 && k < n; ++k)
            if (R.eax == R.args[k])
                R.retarg = (int8_t)k;
        if (R.retarg < 0 && on_stack(R.eax))
            R.retarg = -2;
    }
    for (int k = 0, o = 0; k < g_nregions; o += g_regions[k++].size) {
        const uint8_t* now = (const uint8_t*)g_regions[k].addr;
        for (uint32_t j = 0; j < g_regions[k].size; ++j)
            if (now[j] != g_pre[o + j])
                add_delta(R, 0, o + j, now[j]);
    }
    // A stack buffer's bytes before the call are the caller's leftovers, different in ours: a byte the callee
    // stored but that happened to hold that value already is still a write ours needs (0x739FA0 stores a heap
    // pointer into an uninitialized local whose stale high bytes matched: ours got half a pointer). So the whole
    // span from the first to the last changed byte, widened to dwords, is replayed.
    // A window ends where the next watched pointer starts: a byte belongs to the closest buffer below it (frame
    // layouts differ in ours, so a write into one buffer replayed relative to another lands on anything).
    for (uint32_t k = 0; k < 10; ++k)
        if (g_sptr[k]) {
            const uint8_t* now = (const uint8_t*)g_sptr[k];
            uint32_t m = g_slen[k], lo = m, hi = 0;
            bool dup = false;
            for (uint32_t q = 0; q < 10; ++q) {
                if (q != k && g_sptr[q] > g_sptr[k] && g_sptr[q] - g_sptr[k] < m)
                    m = g_sptr[q] - g_sptr[k];
                dup |= q < k && g_sptr[q] == g_sptr[k];
            }
            if (dup)
                continue;
            for (uint32_t j = 0; j < m; ++j)
                if (now[j] != g_spre[k][j]) {
                    lo = lo < j ? lo : j;
                    hi = j + 1;
                }
            if (lo < hi) {
                lo &= ~3u;
                hi = (hi + 3) & ~3u;
                hi = hi < m ? hi : m;
            }
            for (uint32_t j = lo; j < hi; ++j)
                add_delta(R, (uint8_t)(k == 9 ? 9 : k + 1), j, now[j]);
        }
    for (uint32_t k = 0; k < 10 && !R.heapw; ++k)
        if (g_hptr[k] && memcmp((void*)g_hptr[k], g_hpre[k], kHeapPeek))
            R.heapw = g_heapw = true;
    g_depth = 0;
    return g_realret;
}

// Entry of every intercepted callee: stub `push i; jmp t3_entry`. Stack: [i][return][args...].
__declspec(naked) void t3_entry() {
    __asm {
        pushad
        push esp
        call t3_pre
        add esp, 4
        test eax, eax
        jz pass
        cmp eax, 2
        jne nofp
        fld qword ptr [g_t3_fp]
    nofp:
        popad                          // eax/edx: the replayed results
        mov ecx, dword ptr [g_t3_pop]  // ecx is caller-saved: free here
        lea ecx, [esp + ecx + 4]       // where the return address goes: past i, return, and the popped args
        push eax
        mov eax, dword ptr [esp + 8]   // the return address
        mov dword ptr [ecx], eax
        pop eax
        mov esp, ecx
        ret
    pass:
        popad
        push eax
        mov eax, dword ptr [esp + 4]   // i
        shl eax, 2
        add eax, dword ptr [g_t3_tp]
        mov eax, dword ptr [eax]
        mov dword ptr [esp + 4], eax   // i -> the pass-through target
        pop eax
        ret                            // every register as the caller set it
    }
}

// A recorded callee returns here (t3_pre swapped the return address).
__declspec(naked) void t3_post_stub() {
    __asm {
        cmp dword ptr [g_t3_postfp], 0
        je nofp
        fst qword ptr [g_t3_fp]        // keep st0 on the FPU stack: the caller pops it
    nofp:
        push eax                       // slot for the real return address
        pushad
        push esp
        call t3_post
        add esp, 4
        mov dword ptr [esp + 32], eax
        popad
        ret
    }
}
}  // extern "C"

namespace hook::t3 {

bool busy() { return g_mode != OFF && GetCurrentThreadId() == g_thread; }

bool elsewhere() { return g_mode != OFF && GetCurrentThreadId() != g_thread; }

// F's snapshot regions: this (ecx at entry, this_size bytes) and the globals it writes.
void begin_record(uint32_t self, uint32_t this_size, const uint32_t (*globs)[2], uint32_t nglobs) {
    g_nregions = 0;
    uint32_t total = 0;
    if (this_size)
        g_regions[g_nregions++] = {self, this_size};
    for (uint32_t k = 0; k < nglobs && g_nregions < 9; ++k)
        g_regions[g_nregions++] = {globs[k][0], globs[k][1]};
    for (int k = 0; k < g_nregions; ++k)
        total += g_regions[k].size;
    g_nrec = g_cursor = 0;
    g_lost = total > sizeof g_pre;
    g_heapw = false;  // regions too big to watch: record nothing, conclude nothing
    g_mismatch = false;
    g_abort = 0;
    g_msg[0] = 0;
    g_depth = 0;
    g_thread = GetCurrentThreadId();
    g_mode = RECORD;
}

bool lost() { return g_lost; }

void** jmpbuf() { return g_jb; }

void begin_replay() {
    g_cursor = 0;
    g_depth = 0;
    g_mode = REPLAY;
}

// 0: the traces agree, 1: mismatch (msg), 2: inconclusive (a record was lost, or the recording couldn't answer
// ours). After a 1 or 2 the caller keeps the original's results: its callees' side effects are the ones in effect.
int end(char* msg, size_t n) {
    g_mode = OFF;
    if (g_lost) {
        snprintf(msg, n, "more calls, callee writes or snapshot bytes than recorded");
        return 2;
    }
    if (g_abort == 2) {
        snprintf(msg, n, "a callee returned an address in the original's frame");
        return 2;
    }
    if (!g_mismatch && g_cursor < g_nrec)
        mismatch("ours made %d calls, the original %d (next: %08X)", g_cursor, g_nrec,
                 kCallees[g_recs[g_cursor].callee].addr);
    if (g_mismatch && g_heapw) {
        snprintf(msg, n, "a callee changed heap memory ours may read; %s", g_msg);
        return 2;
    }
    if (g_mismatch) {
        snprintf(msg, n, "%s", g_msg);
        return 1;
    }
    return 0;
}

void pause() { g_mode = OFF; }

// After every other hook is in place: wrap each callee's entry.
void install() {
    if (!kNCallees)
        return;
    g_t3_tp = new void*[kNCallees];
    g_stubs = VirtualAlloc(nullptr, 16 * kNCallees, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    int wrapped = 0, tramp = 0, failed = 0;
    for (int i = 0; i < kNCallees; ++i) {
        uint8_t* s = (uint8_t*)g_stubs + 16 * i;
        s[0] = 0x68;  // push imm32 i
        memcpy(s + 1, &i, 4);
        s[5] = 0xE9;  // jmp t3_entry
        int32_t rel = (int32_t)((uintptr_t)&t3_entry - (uintptr_t)(s + 10));
        memcpy(s + 6, &rel, 4);
        uint8_t* e = (uint8_t*)kCallees[i].addr;
        if (e[0] == 0xE9) {  // already patched (our hook / diff stub / MinHook) or an original jmp thunk
            g_t3_tp[i] = e + 5 + *(int32_t*)(e + 1);
            uint8_t jmp[5] = {0xE9};
            int32_t r2 = (int32_t)((uintptr_t)s - ((uintptr_t)e + 5));
            memcpy(jmp + 1, &r2, 4);
            DWORD old;
            VirtualProtect(e, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(e, jmp, 5);
            VirtualProtect(e, 5, old, &old);
            FlushInstructionCache(GetCurrentProcess(), e, 5);
            ++wrapped;
        } else if (MH_CreateHook(e, s, &g_t3_tp[i]) == MH_OK && MH_QueueEnableHook(e) == MH_OK) {
            ++tramp;
        } else {
            g_t3_tp[i] = e;  // can't intercept: never answered (its calls show up as trace mismatches)
            ++failed;
            log("t3: FAILED to intercept %08x", (unsigned)kCallees[i].addr);
        }
    }
    if (MH_ApplyQueued() != MH_OK)
        log("t3: FAILED MH_ApplyQueued");
    log("t3: %d callees intercepted (%d wrapped hooks, %d trampolines, %d failed)", wrapped + tramp, wrapped, tramp,
        failed);
}

}  // namespace hook::t3
