#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* , int index));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(char, __fastcall, GfxScene_ProcessSubsystemIfPresent, 0x006CAC60, (void* self, void* , int a2));
CSF_FUNC(int, __fastcall, SceneFrame_SetFloatParam, 0x006D2AE0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, S_006b1450_Refresh, 0x006B1450, (void* self, void* edx));

static void* PointerFromWord(uint32_t word)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(word));
}

static char __fastcall S_006b1450_ProcessSceneFrame(S_006b1450* self, void* /*edx*/, float a2)
{
    (void)CSF_VTBL(0x006B1450);

    uint32_t globalValue = static_cast<uint32_t>(GetGlobalValue_6C5760());
    C_0085e7b8* sceneObj = CSF_AT(C_0085e7b8*, PointerFromWord(globalValue), 4);
    int sceneRegion = C_0085e7b8_GetSceneRegionAt(sceneObj, nullptr, 0);
    GfxSkin_SetKeyedValue(PointerFromWord(static_cast<uint32_t>(sceneRegion)), nullptr, 14, 0);

    int a2Bits;
    std::memcpy(&a2Bits, &a2, sizeof a2Bits);
    char result = GfxScene_ProcessSubsystemIfPresent(
        PointerFromWord(self->field_005c), nullptr, a2Bits);

    float selectedValue;
    std::memcpy(&selectedValue, &self->field_0030, sizeof selectedValue);
    void* sceneChild = PointerFromWord(CSF_AT(uint32_t, PointerFromWord(self->field_005c), 0xBC));
    if (CSF_AT(uint8_t, sceneChild, 0x34) != 0)
    {
        std::memcpy(&selectedValue, &self->field_004c, sizeof selectedValue);
    }

    SceneFrame_SetFloatParam(PointerFromWord(static_cast<uint32_t>(a2Bits)), nullptr, selectedValue);
    S_006b1450_Refresh(self, nullptr);
    return result;
}

HOOK(0x006B18A0, S_006b1450_ProcessSceneFrame, "auto");
