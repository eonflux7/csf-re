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

namespace hook {

enum class Kind : uint8_t { Jmp, VSlot };

struct Entry {
    uintptr_t addr;   // function entry (Jmp) or vtable address (VSlot)
    uint32_t slot;    // VSlot only
    void* fn;         // replacement
    const char* name;
    const char* module;
    Kind kind;
    Entry* next;
};

void add(Entry* e);
void install_all();
void log(const char* fmt, ...);

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
    Registrar(uintptr_t addr, uint32_t slot, void* fn, const char* name, const char* module, Kind k)
        : e{addr, slot, fn, name, module, k, nullptr} { add(&e); }
};

}  // namespace hook

#define HOOK_CAT2(a, b) a##b
#define HOOK_CAT(a, b) HOOK_CAT2(a, b)
#define HOOK(addr, fn, module) \
    static ::hook::Registrar HOOK_CAT(hook_reg_, __LINE__)(addr, 0, (void*)&fn, #fn, module, ::hook::Kind::Jmp)
#define HOOK_METHOD(addr, mfn, module) \
    static ::hook::Registrar HOOK_CAT(hook_reg_, __LINE__)(addr, 0, ::hook::to_ptr(mfn), #mfn, module, ::hook::Kind::Jmp)
#define HOOK_VSLOT(vtbl, slot, mfn, module) \
    static ::hook::Registrar HOOK_CAT(hook_reg_, __LINE__)(vtbl, slot, ::hook::to_ptr(mfn), #mfn, module, ::hook::Kind::VSlot)
