#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Heavy-enemy score = signed counter at +0x28 (negative values get the unsigned bias added)
// multiplied by a float scale stored in the global value block at +0x1D58.
static double __fastcall MissionInfo_GetHeavyEnemyScore(uint32_t* self, void* /*edx*/) {
    uint8_t* globalBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));

    int32_t storedCount = CSF_AT(int32_t, self, 0x28);
    double scoreValue = static_cast<double>(storedCount);
    if (CSF_AT(int32_t, self, 0x28) < 0) {
        scoreValue += static_cast<double>(g_UnsignedIntegerBias);
    }

    float heavyScale = CSF_AT(float, globalBlock, 0x1D58);
    return scoreValue * heavyScale;
}

HOOK(0x444E20, MissionInfo_GetHeavyEnemyScore, "auto");
