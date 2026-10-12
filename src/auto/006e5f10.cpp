#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(double, __cdecl, GfxSkin_GetLinkedValueOrZero, 0x006C56B0, (int object, int context));

// Original: push esi; esi = this; call GetGlobalValue; push [eax+0xF0]; push [esi+4];
// call GfxSkin_GetLinkedValueOrZero; add esp,8; pop esi; ret.
// eax is not set after the call, so the callee's st0 result is the return value.
static double __fastcall GfxObjectInstance_QueryLinkedValue(void* self, void* /*edx*/)
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    int object = CSF_AT(int, self, 0x04);
    int context = CSF_AT(int, globalValue, 0xF0);
    return GfxSkin_GetLinkedValueOrZero(object, context);
}

HOOK(0x006E5F10, GfxObjectInstance_QueryLinkedValue, "auto");
