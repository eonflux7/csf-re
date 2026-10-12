#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Officer score = signed counter at +0x18 (negative values get the unsigned bias added)
// multiplied by a float scale stored in the global value block at +0x1D50.
static double __fastcall MissionInfo_GetOfficerScore(uint32_t* self, void* /*edx*/) {
    uint8_t* globalBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));

    int32_t storedCount = CSF_AT(int32_t, self, 0x18);
    double officerValue = static_cast<double>(storedCount);
    if (storedCount < 0) {
        officerValue += static_cast<double>(g_UnsignedIntegerBias);
    }

    float officerScale = CSF_AT(float, globalBlock, 0x1D50);
    return officerValue * officerScale;
}

HOOK(0x444DC0, MissionInfo_GetOfficerScore, "auto");
