// Reimplementation of sub_52BDF0 (0x0052BDF0) -- menu_progress_input.
//
// Takes the object (this), grabs a global value and updates it through
// 0x005225D0, then asks the child object stored at this+0x0C to set its
// flag/virtual at +0x9C to 1 and finally tail-forwards to that same
// child's virtual slot at +0xA8, returning its result unchanged.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// int __cdecl sub_521BB0() -- returns the (pointer to the) global value.
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());

// int __thiscall sub_5225D0(std::uint32_t* this) -- updates the global value.
CSF_FUNC(int, __fastcall, Menu_5225D0, 0x5225D0, (void* self, void* /*edx*/));

// Tail-forwarding thunk: ecx = child object, jumps through its vtable slot
// +0xA8, handing the original caller's return address directly to the callee.
static __declspec(naked) void CMenuProgressBar_JumpToVtblA8()
{
    __asm
    {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0xa8]
    }
}

static int __fastcall CMenuProgressBar_HandleInputEvent(uint32_t** self, void* /*edx*/)
{
    // v2 = GetGlobalValue();
    // sub_5225D0(v2);
    int v2 = GetGlobalValue_521BB0();
    Menu_5225D0(reinterpret_cast<void*>(v2), nullptr);

    // ecx = this[3]; eax = [ecx]; push 1; call [eax + 0x9C];
    void* child = CSF_AT(void*, self, 0xC);
    void** childVtbl = *reinterpret_cast<void***>(child);
    (reinterpret_cast<void (__fastcall*)(void*, void*, int)>(childVtbl[0x9C / 4]))(child, nullptr, 1);

    // ecx = this[3]; edx = [ecx]; jmp [edx + 0xA8];
    // (the field is re-read because the call above may have replaced it)
    void* childAfterCall = CSF_AT(void*, self, 0xC);
    return (reinterpret_cast<int (__fastcall*)(void*, void*)>(CMenuProgressBar_JumpToVtblA8))(
        childAfterCall, nullptr);
}

HOOK(0x52BDF0, CMenuProgressBar_HandleInputEvent, "auto");
