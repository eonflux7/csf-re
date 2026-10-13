#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorOffset, 0x007F8340, (uint32_t* a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorMultiplier, 0x007F8390, (uint32_t* a1, int a2));

// __thiscall(C_00861a78* self, int a2, int a4), callee pops 8 bytes.
static uint32_t* __fastcall Im3dObject_ApplyColorMultiplierAndOffset(C_00861a78* self, void* /*edx*/, int a2, int a4)
{
    // The field holds a pointer to a wrapper whose first dword is the Rt2d object pointer.
    uint32_t* rt2dObject = *reinterpret_cast<uint32_t**>(self->field_0004);

    Rt2dObjectSetColorMultiplier(rt2dObject, a4);
    // The original pushes two arguments only and cleans both calls up at once (add esp, 0x10): the third and
    // fourth dwords of the bound prototype are the multiplier call's arguments, still on the stack.
    return Rt2dObjectSetColorOffset(rt2dObject, a2, static_cast<int>(reinterpret_cast<uintptr_t>(rt2dObject)), a4);
}

HOOK(0x006E3D70, Im3dObject_ApplyColorMultiplierAndOffset, "auto");
