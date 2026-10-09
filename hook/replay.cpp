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
#include <windows.h>
#include <intrin.h>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "MinHook.h"
#include "hook.h"

namespace {

struct Callee { uint32_t addr, pop, nargs, regs, ret; };  // regs: 1 ecx, 2 edx; ret: 0 int32 1 fp 3 void 4 int64
const Callee kCallees[] = {
#define CSF_T3_CALLEES
#include "replay_table.inc"
#undef CSF_T3_CALLEES
    {0, 0, 0, 0, 0},
};
constexpr int kNCallees = sizeof kCallees / sizeof kCallees[0] - 1;

struct Regs { uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; };  // pushad layout

enum Mode { OFF, RECORD, REPLAY };

constexpr int kMaxRecs = 64, kMaxDeltas = 128, kPeek = 64;  // kPeek: bytes watched behind a stack pointer arg
struct Delta { uint32_t off; uint8_t where, val; };       // where: 0 F's regions, 1..8 stack arg k-1, 9 ecx
struct Rec {
    uint32_t callee, ecx, edx, args[8], eax, edxr;
    double fp;
    uint32_t nd;
    bool overflow;
    Delta d[kMaxDeltas];
};

Rec g_recs[kMaxRecs];
int g_nrec, g_cursor;
bool g_lost;  // a record didn't fit (calls or deltas): the comparison is inconclusive
Mode g_mode;
DWORD g_thread;
int g_depth;
uint32_t g_realret;
uint8_t g_pre[16384], g_spre[10][kPeek];
uint32_t g_sptr[10];  // stack pointers watched during the current recorded call (0 = none)
char g_msg[256];
bool g_mismatch;

struct Region { uint32_t addr, size; };
Region g_regions[9];
int g_nregions;

void* g_stubs;

bool on_stack(uint32_t p) {
    return p >= __readfsdword(8) && p < __readfsdword(4);  // TEB StackLimit .. StackBase
}

uint32_t peek_len(uint32_t p) {
    uint32_t top = __readfsdword(4);
    return top - p < kPeek ? top - p : kPeek;
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

bool same_arg(uint32_t o, uint32_t u) {
    return o == u || (on_stack(o) && on_stack(u));  // locals live at other addresses in ours
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
        if (g_nrec >= kMaxRecs) {
            g_lost = true;
            return 0;
        }
        Rec& R = g_recs[g_nrec++];
        R = Rec{};
        R.callee = i;
        R.ecx = r->ecx;
        R.edx = r->edx;
        memcpy(R.args, a, 4 * c.nargs);
        region_copy(g_pre);
        for (uint32_t k = 0; k < 10; ++k) {
            uint32_t p = k < 8 ? (k < c.nargs ? R.args[k] : 0) : k == 9 && (c.regs & 1) ? r->ecx : 0;
            g_sptr[k] = p && on_stack(p) && p > (uint32_t)a ? p : 0;  // a caller's local, not the callee's frame
            if (g_sptr[k])
                memcpy(g_spre[k], (void*)p, peek_len(p));
        }
        g_t3_postfp = c.ret == 1;
        g_realret = *(uint32_t*)((uint8_t*)r + 36);
        *(uint32_t*)((uint8_t*)r + 36) = (uint32_t)(void*)&t3_post_stub;  // the callee returns to us first
        g_depth = 1;
        return 0;
    }
    // REPLAY
    // Diverged: this call runs for real (ours is what the game executes anyway), and replay stops for the rest of
    // the run, or the real callee's own nested calls would be answered from the recording.
    if (g_cursor >= g_nrec) {
        mismatch("call %d: ours calls %08X, the original made only %d calls", g_cursor + 1, c.addr, g_nrec);
        g_mode = OFF;
        return 0;
    }
    Rec& R = g_recs[g_cursor];
    if (R.callee != i) {
        mismatch("call %d: ours calls %08X, the original %08X", g_cursor + 1, c.addr, kCallees[R.callee].addr);
        g_mode = OFF;
        return 0;
    }
    ++g_cursor;
    if ((c.regs & 1) && !same_arg(R.ecx, r->ecx))
        mismatch("call %d to %08X: ecx orig %08X ours %08X", g_cursor, c.addr, R.ecx, r->ecx);
    if ((c.regs & 2) && !same_arg(R.edx, r->edx))
        mismatch("call %d to %08X: edx orig %08X ours %08X", g_cursor, c.addr, R.edx, r->edx);
    for (uint32_t k = 0; k < c.nargs; ++k)
        if (!same_arg(R.args[k], a[k])) {
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
    r->eax = R.eax;
    r->edx = R.edxr;
    g_t3_pop = c.pop;
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
    R.edxr = r->edx;
    R.fp = g_t3_postfp ? g_t3_fp : 0;
    for (int k = 0, o = 0; k < g_nregions; o += g_regions[k++].size) {
        const uint8_t* now = (const uint8_t*)g_regions[k].addr;
        for (uint32_t j = 0; j < g_regions[k].size; ++j)
            if (now[j] != g_pre[o + j])
                add_delta(R, 0, o + j, now[j]);
    }
    for (uint32_t k = 0; k < 10; ++k)
        if (g_sptr[k]) {
            const uint8_t* now = (const uint8_t*)g_sptr[k];
            for (uint32_t j = 0, m = peek_len(g_sptr[k]); j < m; ++j)
                if (now[j] != g_spre[k][j])
                    add_delta(R, (uint8_t)(k == 9 ? 9 : k + 1), j, now[j]);
        }
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

// F's snapshot regions: this (ecx at entry, this_size bytes) and the globals it writes.
void begin_record(uint32_t self, uint32_t this_size, const uint32_t (*globs)[2], uint32_t nglobs) {
    g_nregions = 0;
    if (this_size)
        g_regions[g_nregions++] = {self, this_size};
    for (uint32_t k = 0; k < nglobs && g_nregions < 9; ++k)
        g_regions[g_nregions++] = {globs[k][0], globs[k][1]};
    g_nrec = g_cursor = 0;
    g_lost = g_mismatch = false;
    g_msg[0] = 0;
    g_depth = 0;
    g_thread = GetCurrentThreadId();
    g_mode = RECORD;
}

void begin_replay() {
    g_cursor = 0;
    g_depth = 0;
    g_mode = REPLAY;
}

// 0: the traces agree, 1: mismatch (msg), 2: inconclusive (a record was lost)
int end(char* msg, size_t n) {
    g_mode = OFF;
    if (g_lost)
        return 2;
    if (!g_mismatch && g_cursor < g_nrec)
        mismatch("ours made %d calls, the original %d (next: %08X)", g_cursor, g_nrec,
                 kCallees[g_recs[g_cursor].callee].addr);
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
