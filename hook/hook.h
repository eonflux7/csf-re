// Hook registry. Reimplemented functions register themselves next to their definition:
//
//   HOOK(0x4A1230, MyFreeFunction, "module");
//   HOOK_METHOD(0x4A1300, &CPed::SetHealth, "ped");     // non-virtual __thiscall
//   HOOK_VSLOT(0x830600, 3, &CPed::Update, "ped");      // vtable slot patch
//
// At DLL load, every enabled hook is installed: a 5-byte JMP at the original entry (or a vtable slot write).
// csf_hook.ini (next to the exe) can disable hooks by address or module, for batch bisecting.
#pragma once
#include <cstdint>
#ifdef CSF_STANDALONE
#include "globals.h"  // csf::fn_slot
#endif

namespace hook {

enum class Kind : uint8_t { Jmp, VSlot, Tramp };

struct Entry {
    uintptr_t addr;   // function entry (Jmp) or vtable address (VSlot)
    uint32_t slot;    // VSlot only
    void* fn;         // replacement
    const char* name;
    const char* module;
    Kind kind;
    Entry* next;
    void** orig;      // Tramp only: receives a callable copy of the original (relocated prologue + jmp back)
};

void add(Entry* e);
namespace t3 {  // hook/replay.cpp: diff tier 3 (record/replay of callees)
bool busy();       // a record/replay runs on this thread
bool elsewhere();  // ... on another thread (it holds the diff lock while arbitrary callees run)
void begin_record(uint32_t self, uint32_t this_size, const uint32_t (*globs)[2], uint32_t nglobs);
void pause();
bool lost();       // the recording is incomplete: don't run ours against it
void** jmpbuf();   // __builtin_setjmp buffer: the replayer abandons ours by longjmp-ing to it
void begin_replay();
int end(char* msg, size_t n);
void install();
}  // namespace t3
void install_all();
void log(const char* fmt, ...);
// Rest of the first csf_hook.ini line starting with `key ` (trimmed), or nullptr. Static storage.
const char* ini_string(const char* key);
// Scripted key press (DIK name, e.g. "RETURN") delay_ms from now; see hook/autopilot.cpp.
void autopilot_press(const char* key, unsigned delay_ms);

template <typename T>
inline void* to_ptr(T member) {
    // Non-virtual member function pointers in single-inheritance classes are plain code pointers (MSVC ABI).
    static_assert(sizeof(T) == sizeof(void*), "member pointer must be single-inheritance");
    union { T m; void* p; } u;
    u.m = member;
    return u.p;
}

struct Registrar {
    Entry e;
    Registrar(uintptr_t addr, uint32_t slot, void* fn, const char* name, const char* module, Kind k,
              void** orig = nullptr)
        : e{addr, slot, fn, name, module, k, nullptr, orig} { add(&e); }
};

}  // namespace hook

#define HOOK_CAT2(a, b) a##b
#define HOOK_CAT(a, b) HOOK_CAT2(a, b)
#ifdef CSF_STANDALONE
// Cutover: no patching. HOOK/HOOK_METHOD define the address slot every CSF_FUNC(.., addr, ..) calls through
// (src/globals.h); vtable slots are filled by the generated csf::vt tables; a trampoline's original is gone.
#define HOOK(addr, fn, module) template <> void* const ::csf::fn_slot<(addr)>::p = (void*)&fn
#define HOOK_METHOD(addr, mfn, module) template <> void* const ::csf::fn_slot<(addr)>::p = ::hook::to_ptr(mfn)
#define HOOK_TRAMP(addr, fn, orig, module) HOOK(addr, fn, module)
#define HOOK_VSLOT(vtbl, slot, mfn, module) static_assert(true)
#else
#define HOOK(addr, fn, module) \
    static ::hook::Registrar HOOK_CAT(hook_reg_, __LINE__)(addr, 0, (void*)&fn, #fn, module, ::hook::Kind::Jmp)
#define HOOK_METHOD(addr, mfn, module) \
    static ::hook::Registrar HOOK_CAT(hook_reg_, __LINE__)(addr, 0, ::hook::to_ptr(mfn), #mfn, module, ::hook::Kind::Jmp)
// Wrap a function and keep the original callable through `orig` (a function pointer variable of the right type).
// The prologue (>= 5 bytes of simple instructions, no relative branches) is relocated into a trampoline.
#define HOOK_TRAMP(addr, fn, orig, module) \
    static ::hook::Registrar HOOK_CAT(hook_reg_, __LINE__)(addr, 0, (void*)&fn, #fn, module, ::hook::Kind::Tramp, \
                                                           (void**)&orig)
#define HOOK_VSLOT(vtbl, slot, mfn, module) \
    static ::hook::Registrar HOOK_CAT(hook_reg_, __LINE__)(vtbl, slot, ::hook::to_ptr(mfn), #mfn, module, ::hook::Kind::VSlot)
#endif  // CSF_STANDALONE

// A function whose LTCG callers keep a value in ecx across the call (0x468310: `mov ecx, edi; call 0x463FA0;
// ... call 0x467CC0` with no reload): the original never writes ecx, compiled C++ does. The stub saves ecx, copies
// the `nargs` stack dwords, calls fn (a __fastcall impl popping them, as the original's `ret 4*nargs`), restores ecx.
// edx goes through untouched; eax / edx:eax / st0 results are fn's.
#define HOOK_KEEP_ECX_(addr, fn, nargs, cleanup, retbytes, module) \
    static void* const HOOK_CAT(fn, _impl) = (void*)&fn; \
    static __declspec(naked) void HOOK_CAT(fn, _keep_ecx)() { \
        __asm push ecx __asm push esi __asm push edi \
        __asm sub esp, (4 * (nargs)) \
        __asm lea esi, [esp + (4 * (nargs)) + 16] \
        __asm mov edi, esp \
        __asm mov ecx, (nargs) \
        __asm rep movsd \
        __asm mov ecx, [esp + (4 * (nargs)) + 8] \
        __asm call dword ptr [HOOK_CAT(fn, _impl)] \
        __asm add esp, (cleanup) \
        __asm pop edi __asm pop esi __asm pop ecx \
        __asm ret (retbytes) \
    } \
    HOOK(addr, HOOK_CAT(fn, _keep_ecx), module)
// fn pops its arguments itself (__stdcall / __thiscall-as-__fastcall): the wrapper pops the caller's copy too
#define HOOK_KEEP_ECX(addr, fn, nargs, module) HOOK_KEEP_ECX_(addr, fn, nargs, 0, 4 * (nargs), module)
// fn is __cdecl: it leaves the wrapper's copy of the arguments on the stack (the wrapper drops it) and the caller
// pops its own (the wrapper must not). 0x4068E0 under HOOK_KEEP_ECX popped the copy as its saved registers.
#define HOOK_KEEP_ECX_CDECL(addr, fn, nargs, module) HOOK_KEEP_ECX_(addr, fn, nargs, 4 * (nargs), 0, module)

// A function with arguments in registers that no calling convention names (LTCG: eax / edi / esi / ebx; 0x6A4610
// takes edx, edi and one stack dword). fn is `uint32_t __cdecl fn(hook::Regs* regs, const uint32_t* args)`: regs
// holds every register as it was at entry, args the caller's stack arguments (read only). fn's result is the
// original's eax; every other register goes back to the caller as regs holds it when fn returns (so unchanged
// unless fn writes regs: LTCG callers rely on registers the original never touches). retbytes is the N of the
// original's `ret N` (0 for a plain `ret`).
namespace hook {
struct Regs { uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; };  // pushad order
// Call a function that takes register arguments: loads eax, ecx, edx, ebx, esi, edi from `in`, pushes the `nstack`
// dwords of `stack` (stack[0] is the first argument) and calls fn; works whether fn pops them or its caller does.
__declspec(naked) inline uint32_t __cdecl call_regs(uintptr_t /*fn*/, const Regs* /*in*/, const uint32_t* /*stack*/,
                                                    unsigned /*nstack*/) {
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        mov ecx, [ebp + 20]
        mov esi, [ebp + 16]
        lea eax, [ecx * 4]
        sub esp, eax
        mov edi, esp
        rep movsd
        mov eax, [ebp + 12]
        mov edi, [eax]
        mov esi, [eax + 4]
        mov ebx, [eax + 16]
        mov edx, [eax + 20]
        mov ecx, [eax + 24]
        mov eax, [eax + 28]
        call dword ptr [ebp + 8]
        lea esp, [ebp - 12]
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
}  // namespace hook
#define HOOK_REGS(addr, fn, retbytes, module) \
    static void* const HOOK_CAT(fn, _impl) = (void*)&fn; \
    static __declspec(naked) void HOOK_CAT(fn, _regs)() { \
        __asm pushad \
        __asm lea eax, [esp + 36] \
        __asm push eax \
        __asm lea eax, [esp + 4] \
        __asm push eax \
        __asm call dword ptr [HOOK_CAT(fn, _impl)] \
        __asm add esp, 8 \
        __asm mov [esp + 28], eax \
        __asm popad \
        __asm ret (retbytes) \
    } \
    HOOK(addr, HOOK_CAT(fn, _regs), module)

// A function that ends in an indirect tail jump (`mov ecx, obj; jmp [vtbl + N]`): the target's signature is unknown
// and it gets the caller's stack arguments and return address untouched, which C++ can't write. fn is everything
// before the jump: `hook::Tail __fastcall fn(void* self, const uint32_t* args)` (args: the caller's stack arguments,
// read only). It returns hook::tail_jump(ecx for the target, target) or, on a path where the original returns
// itself, hook::tail_ret(eax) for a plain `ret` and hook::tail_ret(eax, N) for `ret N` (N = 4..32, the caller's
// stack arguments the original pops there). fn never takes more than those two parameters: a third one makes fn
// pop stack arguments the jump target still needs.
namespace hook {
using Tail = uint64_t;  // edx:eax = target : ecx for the target
__declspec(naked) inline void tail_ret_stub() { __asm { mov eax, ecx } __asm { ret } }
#define CSF_TAIL_RET_STUB(n) \
    __declspec(naked) inline void tail_ret_stub_##n() { __asm { mov eax, ecx } __asm { ret n } }
CSF_TAIL_RET_STUB(4) CSF_TAIL_RET_STUB(8) CSF_TAIL_RET_STUB(12) CSF_TAIL_RET_STUB(16)
CSF_TAIL_RET_STUB(20) CSF_TAIL_RET_STUB(24) CSF_TAIL_RET_STUB(28) CSF_TAIL_RET_STUB(32)
#undef CSF_TAIL_RET_STUB
inline Tail tail_jump(void* self, void* target) {
    return (uint64_t)(uintptr_t)self | ((uint64_t)(uintptr_t)target << 32);
}
inline Tail tail_ret(uint32_t eax = 0) { return eax | ((uint64_t)(uintptr_t)&tail_ret_stub << 32); }
inline Tail tail_ret(uint32_t eax, unsigned pop) {
    static void (*const stubs[9])() = {&tail_ret_stub, &tail_ret_stub_4, &tail_ret_stub_8, &tail_ret_stub_12,
                                       &tail_ret_stub_16, &tail_ret_stub_20, &tail_ret_stub_24, &tail_ret_stub_28,
                                       &tail_ret_stub_32};
    return eax | ((uint64_t)(uintptr_t)stubs[pop / 4] << 32);
}
}  // namespace hook
#define HOOK_TAIL(addr, fn, module) \
    static void* const HOOK_CAT(fn, _impl) = (void*)&fn; \
    static __declspec(naked) void HOOK_CAT(fn, _tail)() { \
        __asm lea edx, [esp + 4] \
        __asm call dword ptr [HOOK_CAT(fn, _impl)] \
        __asm mov ecx, eax \
        __asm jmp edx \
    } \
    HOOK(addr, HOOK_CAT(fn, _tail), module)
