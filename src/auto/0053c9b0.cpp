// Reimplementation of sub_53C9B0 (0x0053C9B0) -- menu_sound_options.
//
// Fetches the global value via 0x521BB0 and hands it to 0x5225D0, then asks the
// child object stored at this+0x10 to run its virtual slot +0x9C with argument 1,
// and finally tail-forwards to that same child's virtual slot +0xA8, returning
// its result unchanged.

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
// +0xA8, passing the original caller's return address straight to the callee.
static __declspec(naked) void CMenuSoundOptions_JumpToVtblA8()
{
    __asm
    {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0xa8]
    }
}

static int __fastcall CMenuSoundOptions_HandleInputEvent(uint32_t** self, void* /*edx*/)
{
    // v2 = GetGlobalValue();
    // sub_5225D0(v2);
    int v2 = GetGlobalValue_521BB0();
    Menu_5225D0(reinterpret_cast<void*>(v2), nullptr);

    // ecx = [esi + 0x10]; eax = [ecx]; push 1; call [eax + 0x9C];
    void* child = CSF_AT(void*, self, 0x10);
    void** childVtbl = *reinterpret_cast<void***>(child);
    (reinterpret_cast<void (__fastcall*)(void*, void*, int)>(childVtbl[0x9C / 4]))(child, nullptr, 1);

    // ecx = [esi + 0x10]; edx = [ecx]; jmp [edx + 0xA8];
    // (the field is re-read because the call above may have replaced it)
    void* childAfterCall = CSF_AT(void*, self, 0x10);
    return (reinterpret_cast<int (__fastcall*)(void*, void*)>(CMenuSoundOptions_JumpToVtblA8))(
        childAfterCall, nullptr);
}

HOOK(0x53C9B0, CMenuSoundOptions_HandleInputEvent, "auto");
