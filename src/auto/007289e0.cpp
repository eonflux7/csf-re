#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00713E80: char __thiscall sub_713E80(int this)  -- shared preparation/refresh routine.
CSF_FUNC(char, __fastcall, C_008657e0_PrepareUpdate, 0x00713E80,
         (C_008657e0* self, void* edx));

// Tail-jump forwarder: ecx = this, dispatch through vtbl slot 0x8c/4 = 35.
static __declspec(naked) int __fastcall C_008657e0_ForwardUpdate(
    C_008657e0* self, void* edx)
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 08Ch]
    }
}

// Calls vtbl slot 8 (0x20); if its low byte is non-zero, runs the shared preparation
// routine and then forwards to vtbl slot 35 (0x8c). Otherwise returns the slot-8 result.
static int __fastcall C_008657e0_ConditionalUpdate(
    C_008657e0* self, void* /*edx*/)
{
    const int result =
        ((int (__fastcall*)(void*, void*))self->vtbl[8])(self, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    C_008657e0_PrepareUpdate(self, nullptr);

    [[clang::musttail]] return C_008657e0_ForwardUpdate(self, nullptr);
}

HOOK(0x007289E0, C_008657e0_ConditionalUpdate, "auto");
