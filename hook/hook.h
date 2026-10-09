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
bool busy();
void begin_record(uint32_t self, uint32_t this_size, const uint32_t (*globs)[2], uint32_t nglobs);
void pause();
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
