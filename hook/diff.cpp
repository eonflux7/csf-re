// Differential testing: run the original and our reimplementation on the same live inputs and compare.
//
// Enabled with csf_hook.ini `diff 1` (optionally `diff_calls N`, default 200 compared calls per function). Only
// functions in build/diff_table.inc (tools/difftable.py: leaf functions whose writes go to `this` or to absolute
// globals) are diffed; the rest are hooked normally.
//
// Per hooked function a stub does: inc [hits]; pushad; push esp; push i; call diff_run_{int,fp}; add esp, 8;
// popad; ret N. diff_run snapshots `this` (the bytes the function can touch) and the written globals, calls the
// original (MinHook trampoline) with the caller's ecx/edx and a copy of the stack args, records the result and
// memory, restores the snapshot, calls ours, and compares. Ours stays in effect; differences go to csf_diff.log.
#include <windows.h>
#include <intrin.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include "MinHook.h"
#include "hook.h"

namespace hook {
extern char g_dir[MAX_PATH];
}

namespace {

struct Glob { uint32_t addr, size; };
// fl: snapshot offsets the original writes with FPU/SSE stores (+0x8000 = double): compared with a relative
// tolerance (x87 80-bit intermediates vs clang's SSE math differ in the last bits; that is not a behavior change).
struct DiffInfo { uint32_t addr, conv, argbytes, ret, this_size, nglobs; Glob globs[8]; uint32_t nfl, fl[8]; };
const DiffInfo kTable[] = {
#include "diff_table.inc"
};
const DiffInfo kT3[] = {  // tier 3: callees recorded and replayed (hook/replay.cpp)
#define CSF_T3_FUNCS
#include "replay_table.inc"
#undef CSF_T3_FUNCS
    {0},
};

struct Regs { uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; };  // pushad layout

struct Slot {
    const DiffInfo* info;
    bool t3;
    uint32_t inconclusive;
    const char* name;
    void* orig;   // MinHook trampoline
    void* ours;
    uint32_t calls, compared, mismatches;
};
Slot* g_slots;
int g_nslots;
uint32_t g_maxCalls = 200;
uint32_t g_limit = 0xFFFFFFFF;  // ini diff_limit N: only the first N table entries (bisecting)
bool g_no_t3 = false;           // ini diff_t3 0: tier 3 off
uint32_t g_t3limit = 0xFFFFFFFF;  // ini diff_t3_limit N: only the first N tier-3 entries (bisecting)
uint32_t g_skip[64]; int g_nskip;  // ini diff_skip A,B,...: addresses never diffed (hex, no 0x needed)
bool g_trace = false;           // ini diff_trace 1: log every diffed call before the original runs (who was last)
constexpr uint32_t kMinArgCopy = 64;  // the invoke thunk restores esp itself, so over-copying args is harmless
CRITICAL_SECTION g_cs;
FILE* g_log;

// Calls fn with ecx/edx set and a copy of the stack args; restores esp itself (works for any callee cleanup).
// Declared without parameters/return so clang emits no prologue/epilogue of its own (it mishandles naked
// functions with non-void returns); used through the typed pointers below.
extern "C" __declspec(naked) void diff_invoke_raw() {
    __asm {
        push ebp
        mov ebp, esp
        push esi
        push edi
        mov ecx, [ebp + 24]
        sub esp, ecx
        mov esi, [ebp + 20]
        mov edi, esp
        rep movsb
        mov ecx, [ebp + 12]
        mov edx, [ebp + 16]
        call dword ptr [ebp + 8]
        lea esp, [ebp - 8]
        pop edi
        pop esi
        pop ebp
        ret
    }
}
using InvokeInt = uint64_t(__cdecl*)(void*, uint32_t, uint32_t, const void*, uint32_t);
using InvokeFp = double(__cdecl*)(void*, uint32_t, uint32_t, const void*, uint32_t);
const InvokeInt diff_invoke_int = (InvokeInt)(void*)&diff_invoke_raw;
const InvokeFp diff_invoke_fp = (InvokeFp)(void*)&diff_invoke_raw;

bool readable(uint32_t p, uint32_t n) {
    if (!p || p < 0x10000)
        return false;
    MEMORY_BASIC_INFORMATION mb;
    if (!VirtualQuery((void*)p, &mb, sizeof mb) || mb.State != MEM_COMMIT || (mb.Protect & (PAGE_NOACCESS | PAGE_GUARD)))
        return false;
    return p + n <= (uint32_t)mb.BaseAddress + mb.RegionSize;
}

struct Snap {
    uint8_t buf[16384];
    uint32_t len = 0;
};

// Regions: [this, this+this_size) and the globals.
bool capture(const Slot& s, const Regs* r, Snap& out) {
    out.len = 0;
    const DiffInfo& d = *s.info;
    if (d.this_size) {
        if (!readable(r->ecx, d.this_size) || d.this_size > 12288)
            return false;
        memcpy(out.buf, (void*)r->ecx, d.this_size);
        out.len = d.this_size;
    }
    for (uint32_t g = 0; g < d.nglobs; ++g) {
        memcpy(out.buf + out.len, (void*)d.globs[g].addr, d.globs[g].size);
        out.len += d.globs[g].size;
    }
    return true;
}

void restore(const Slot& s, const Regs* r, const Snap& in) {
    const DiffInfo& d = *s.info;
    uint32_t o = 0;
    if (d.this_size) {
        memcpy((void*)r->ecx, in.buf, d.this_size);
        o = d.this_size;
    }
    for (uint32_t g = 0; g < d.nglobs; ++g) {
        DWORD old;
        VirtualProtect((void*)d.globs[g].addr, d.globs[g].size, PAGE_READWRITE, &old);
        memcpy((void*)d.globs[g].addr, in.buf + o, d.globs[g].size);
        VirtualProtect((void*)d.globs[g].addr, d.globs[g].size, old, &old);
        o += d.globs[g].size;
    }
}

void report(Slot& s, const Regs* r, const char* what) {
    ++s.mismatches;
    if (s.mismatches > 3 || !g_log)
        return;
    const uint32_t* a = (const uint32_t*)((const uint8_t*)r + 36);
    fprintf(g_log, "DIFF %08X %s: %s | ecx=%08X edx=%08X args=", s.info->addr, s.name, what, r->ecx, r->edx);
    for (uint32_t i = 0; i < s.info->argbytes / 4 && i < 6; ++i)
        fprintf(g_log, "%08X ", a[i]);
    fputc('\n', g_log);
    fflush(g_log);
}

void inconclusive(Slot& s, const char* why) {
    if (s.inconclusive++ < 3 && g_log) {
        fprintf(g_log, "T3 %08X %s: inconclusive (%s)\n", s.info->addr, s.name, why);
        fflush(g_log);
    }
}

bool fp_close(double a, double b, double rel) {
    if (a == b || (a != a && b != b))
        return true;
    double d = a > b ? a - b : b - a, m = (a < 0 ? -a : a) > (b < 0 ? -b : b) ? (a < 0 ? -a : a) : (b < 0 ? -b : b);
    return d <= rel * m || d < 1e-30;
}

// Length of the float/double the original stores at snapshot offset i (0 = none): compared with fp_close.
uint32_t fp_at(const DiffInfo& d, uint32_t i, uint32_t& start) {
    for (uint32_t k = 0; k < d.nfl; ++k) {
        uint32_t o = d.fl[k] & 0x7FFF, n = d.fl[k] & 0x8000 ? 8 : 4;
        if (i >= o && i < o + n) {
            start = o;
            return n;
        }
    }
    return 0;
}

void compare_mem(Slot& s, const Regs* r, const Snap& o, const Snap& u) {
    for (uint32_t i = 0; i < o.len; ++i)
        if (o.buf[i] != u.buf[i]) {
            uint32_t at, n = fp_at(*s.info, i, at);
            if (n && at + n <= o.len) {
                bool close = n == 4 ? fp_close(*(const float*)(o.buf + at), *(const float*)(u.buf + at), 1e-5)
                                    : fp_close(*(const double*)(o.buf + at), *(const double*)(u.buf + at), 1e-12);
                if (close) {
                    i = at + n - 1;
                    continue;
                }
            }
            char w[128];
            snprintf(w, sizeof w, "memory byte %u differs: orig %02X ours %02X", i, o.buf[i], u.buf[i]);
            report(s, r, w);
            return;
        }
}

// Bytes of caller stack to copy: the declared args, at least kMinArgCopy for cdecl (prototypes can undercount),
// never past the top of this thread's stack.
uint32_t argcopy(const Slot& s, const void* args) {
    uint32_t want = s.info->argbytes > kMinArgCopy ? s.info->argbytes : kMinArgCopy;
    uint32_t top = __readfsdword(4);  // TEB StackBase (highest address)
    uint32_t room = top > (uint32_t)args ? top - (uint32_t)args : 0;
    if (want > room)
        want = room < s.info->argbytes ? s.info->argbytes : room;
    return want & ~3u;
}

template <typename R, typename Invoke>
R run(int i, Regs* r, Invoke invoke) {
    Slot& s = g_slots[i];
    const void* args = (const uint8_t*)r + 36;  // past pushad + return address
    if (s.calls++ >= g_maxCalls || hook::t3::busy())  // no nested diffs inside a tier-3 record/replay
        return invoke(s.ours, r->ecx, r->edx, args, argcopy(s, args));
    // A tier-3 diff holds the lock while the original's callees run: they may wait on this thread (the loader)
    while (!TryEnterCriticalSection(&g_cs)) {
        if (hook::t3::elsewhere())
            return invoke(s.ours, r->ecx, r->edx, args, argcopy(s, args));
        Sleep(0);
    }
    // The lock is recursive: a diffed function whose original (or ours) calls another diffed function comes back
    // here on the same thread. The snapshots below are static, so a nested comparison would overwrite the outer
    // one's `before` and the outer object would be restored with the inner one's bytes (2026-10-11: 0x463670 ->
    // 0x46F390 crashed every start once both were in the table). Nested calls just run ours.
    static int depth;
    if (depth) {
        LeaveCriticalSection(&g_cs);
        return invoke(s.ours, r->ecx, r->edx, args, argcopy(s, args));
    }
    struct Depth { Depth() { ++depth; } ~Depth() { --depth; } } nested;
    // static, under the lock: game threads (e.g. the loader) have small stacks
    static Snap before, afterOrig, afterOurs;
    R ro{}, ru{};
    if (!capture(s, r, before)) {
        LeaveCriticalSection(&g_cs);
        return invoke(s.ours, r->ecx, r->edx, args, argcopy(s, args));
    }
    const DiffInfo& d = *s.info;
    if (s.t3)
        hook::t3::begin_record(r->ecx, d.conv == 2 || d.conv == 3 ? d.this_size : 0,
                               (const uint32_t(*)[2])d.globs, d.nglobs);
    if (g_trace && g_log) {
        fprintf(g_log, "RUN %08X %s ecx=%08X arg0=%08X\n", d.addr, s.name, r->ecx, *(const uint32_t*)args);
        fflush(g_log);
    }
    ro = invoke(s.orig, r->ecx, r->edx, args, argcopy(s, args));
    if (s.t3)
        hook::t3::pause();
    char trace[256] = "";
    if (s.t3 && hook::t3::lost()) {  // ours can't run against an incomplete recording: the original's run stands
        hook::t3::end(trace, sizeof trace);
        inconclusive(s, trace);
        LeaveCriticalSection(&g_cs);
        return ro;
    }
    capture(s, r, afterOrig);
    restore(s, r, before);
    if (!s.t3) {
        ru = invoke(s.ours, r->ecx, r->edx, args, argcopy(s, args));
    } else {
        // The replayer abandons ours (longjmp here) where it leaves the recording: unwound without its epilogues,
        // so the SEH chain head and the x87 stack are put back by hand.
        uint32_t seh = __readfsdword(0);
        hook::t3::begin_replay();
        if (__builtin_setjmp(hook::t3::jmpbuf()) == 0) {
            ru = invoke(s.ours, r->ecx, r->edx, args, argcopy(s, args));
        } else {
            __asm {
                mov eax, seh
                mov dword ptr fs:[0], eax
                ffree st(0)
                ffree st(1)
                ffree st(2)
                ffree st(3)
                ffree st(4)
                ffree st(5)
                ffree st(6)
                ffree st(7)
            }
        }
    }
    int t3st = s.t3 ? hook::t3::end(trace, sizeof trace) : 0;
    if (t3st) {
        // The callees' side effects are the original's (made with its arguments, in its order): keep its results.
        restore(s, r, afterOrig);
        if (t3st == 2) {
            inconclusive(s, trace);
        } else {
            ++s.compared;
            report(s, r, trace);
        }
        LeaveCriticalSection(&g_cs);
        return ro;
    }
    capture(s, r, afterOurs);
    ++s.compared;
    compare_mem(s, r, afterOrig, afterOurs);
    if constexpr (!std::is_floating_point_v<R>) {
        // only the bits the original defines: al for bytes/bools, ax for shorts, edx:eax for 64-bit
        uint64_t mask = s.info->ret == 4 ? ~0ull : s.info->ret == 5 ? 0xFFull : s.info->ret == 6 ? 0xFFFFull : 0xFFFFFFFFull;
        if (s.info->ret != 3 && ((uint64_t)ro & mask) != ((uint64_t)ru & mask)) {
            char w[96];
            snprintf(w, sizeof w, "return orig %llX ours %llX", (unsigned long long)(ro & mask),
                     (unsigned long long)(ru & mask));
            report(s, r, w);
        }
    } else {
        bool same = s.info->ret == 1 ? fp_close((float)ro, (float)ru, 1e-5) : fp_close(ro, ru, 1e-12);
        if (!same) {
            char w[96];
            snprintf(w, sizeof w, "fp return orig %.9g ours %.9g", (double)ro, (double)ru);
            report(s, r, w);
        }
    }
    LeaveCriticalSection(&g_cs);
    return ru;
}

// popad restores every register the caller had except the ones the function returns in: callers compiled with
// whole-program optimization may keep live values in registers the original leaf never touches (e.g. edx).
extern "C" void __cdecl diff_run_int(int i, Regs* r) {
    uint64_t v = run<uint64_t>(i, r, diff_invoke_int);
    uint32_t ret = g_slots[i].info->ret;
    if (ret != 3)
        r->eax = (uint32_t)v;
    if (ret == 4)
        r->edx = (uint32_t)(v >> 32);
}

extern "C" double __cdecl diff_run_fp(int i, Regs* r) {
    return run<double>(i, r, diff_invoke_fp);
}

DWORD WINAPI summary_thread(void*) {
    for (;;) {
        Sleep(5000);
        std::string path = std::string(hook::g_dir) + "csf_diff_summary.txt";
        if (FILE* f = fopen(path.c_str(), "w")) {
            for (int i = 0; i < g_nslots; ++i)
                if (g_slots[i].compared || g_slots[i].inconclusive)
                    fprintf(f, "%08X %6u compared %4u mismatched %4u inconclusive  %s\n", g_slots[i].info->addr,
                            g_slots[i].compared, g_slots[i].mismatches, g_slots[i].inconclusive, g_slots[i].name);
            fclose(f);
        }
    }
}

}  // namespace

namespace hook {

// Called by install_all for each Jmp hook when diffing is on. Returns true if the entry was installed as a diff
// hook (stub written at `stub`, which already counts hits), false to install it normally.
bool diff_install(const Entry& e, uint8_t* stub, uint32_t* counter, int max_slots) {
    static bool init = false;
    if (!init) {
        init = true;
        InitializeCriticalSection(&g_cs);
        g_slots = new Slot[max_slots]();
        g_log = fopen((std::string(g_dir) + "csf_diff.log").c_str(), "w");
        if (const char* n = ini_string("diff_calls"))
            g_maxCalls = (uint32_t)strtoul(n, nullptr, 0);
        if (const char* n = ini_string("diff_limit"))
            g_limit = (uint32_t)strtoul(n, nullptr, 0);
        if (const char* n = ini_string("diff_t3"))
            g_no_t3 = atoi(n) == 0;
        if (const char* n = ini_string("diff_t3_limit"))
            g_t3limit = (uint32_t)strtoul(n, nullptr, 0);
        if (const char* n = ini_string("diff_skip"))
            for (char* e; g_nskip < 64 && *n; n = *e ? e + 1 : e) {
                g_skip[g_nskip++] = (uint32_t)strtoul(n, &e, 16);
                if (e == n)
                    break;
            }
        if (const char* n = ini_string("diff_trace"))
            g_trace = atoi(n) != 0;
        CreateThread(nullptr, 0, summary_thread, nullptr, 0, nullptr);
    }
    const DiffInfo* d = nullptr;
    bool t3 = false;
    for (uint32_t k = 0; k < sizeof kTable / sizeof kTable[0] && k < g_limit; ++k)
        if (kTable[k].addr == e.addr)
            d = &kTable[k];
    for (uint32_t k = 0; !d && kT3[k].addr && k < g_t3limit; ++k)
        if (kT3[k].addr == e.addr && !g_no_t3)
            d = &kT3[k], t3 = true;
    for (int k = 0; k < g_nskip; ++k)
        if (g_skip[k] == e.addr)
            d = nullptr;
    if (!d || g_nslots >= max_slots)
        return false;
    int i = g_nslots;
    Slot& s = g_slots[i];
    s.info = d;
    s.t3 = t3;
    s.name = e.name;
    s.ours = e.fn;
    uint8_t* p = stub;
    *p++ = 0xFF; *p++ = 0x05; memcpy(p, &counter, 4); p += 4;            // inc dword [counter]
    *p++ = 0x60;                                                         // pushad
    *p++ = 0x54;                                                         // push esp
    *p++ = 0x68; memcpy(p, &i, 4); p += 4;                               // push i
    void* fn = (d->ret == 1 || d->ret == 2) ? (void*)&diff_run_fp : (void*)&diff_run_int;
    *p++ = 0xE8; int32_t rel = (int32_t)((uintptr_t)fn - ((uintptr_t)p + 4)); memcpy(p, &rel, 4); p += 4;
    *p++ = 0x83; *p++ = 0xC4; *p++ = 0x08;                               // add esp, 8
    *p++ = 0x61;                                                         // popad
    uint16_t n = (uint16_t)(d->conv == 0 ? 0 : d->argbytes);
    if (n) { *p++ = 0xC2; memcpy(p, &n, 2); p += 2; } else *p++ = 0xC3;  // ret N
    if (MH_CreateHook((void*)e.addr, stub, &s.orig) != MH_OK || MH_QueueEnableHook((void*)e.addr) != MH_OK)
        return false;
    ++g_nslots;
    return true;
}

}  // namespace hook
