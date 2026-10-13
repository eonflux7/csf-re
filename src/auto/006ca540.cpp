#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(double, __cdecl, GfxSkin_DistanceBetweenPositions, 0x006C5050, (void* a1, void* a2));

// Original: push esi; esi = this; call GetGlobalValue; push [eax+0xF0]; push [esi+4];
// call GfxSkin_DistanceBetweenPositions; add esp,8; pop esi; ret.
// The callee's st0 (double) is passed straight through as the return value.
static double __fastcall ComputeDistanceToGlobalTarget(void* self, void* /*edx*/)
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    uint32_t globalSetting = CSF_AT(uint32_t, globalValue, 0xF0);
    uint32_t widgetPosition = CSF_AT(uint32_t, self, 0x04);
    return GfxSkin_DistanceBetweenPositions(reinterpret_cast<void*>(widgetPosition),
                                            reinterpret_cast<void*>(globalSetting));
}

HOOK(0x006CA540, ComputeDistanceToGlobalTarget, "auto");
