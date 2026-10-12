#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(double, __cdecl, GfxSkin_GetPointDistance, 0x006C4F70, (int a1, int a2));

// Original: push esi; esi = this; call GetGlobalValue; push [eax+0xF0]; push [esi+4];
// call GfxSkin_GetPointDistance; add esp,8; pop esi; ret. The callee's st0 is the return value.
static double __fastcall GfxObject_ComputePointDistance(void* self, void* /*edx*/)
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    int globalSetting = CSF_AT(int, globalValue, 0xF0);
    int objectPointer = CSF_AT(int, self, 0x04);
    return GfxSkin_GetPointDistance(objectPointer, globalSetting);
}

HOOK(0x006B3CD0, GfxObject_ComputePointDistance, "auto");
