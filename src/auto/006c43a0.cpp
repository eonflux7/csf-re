#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(double, __cdecl, GfxSkin_ComputeDistanceToTarget, 0x006C50B0, (void* a1, void* a2));

// Original: push esi; esi = this; call GetGlobalValue; push [eax+0xF0]; push [esi+4];
// call GfxSkin_ComputeDistanceToTarget; add esp,8; pop esi; ret.
// The callee's st0 (double) is the return value and passes through unchanged.
static double __fastcall SkinQueryDistanceToGlobalTarget(void* self, void* /*edx*/)
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    uint32_t skinTarget = CSF_AT(uint32_t, self, 0x04);
    uint32_t targetSetting = CSF_AT(uint32_t, globalValue, 0xF0);
    return GfxSkin_ComputeDistanceToTarget(reinterpret_cast<void*>(skinTarget),
                                           reinterpret_cast<void*>(targetSetting));
}

HOOK(0x006C43A0, SkinQueryDistanceToGlobalTarget, "auto");
