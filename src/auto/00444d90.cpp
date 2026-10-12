#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Elimination score = signed counter at +0x10 (negative values get the unsigned bias added)
// multiplied by a float scale stored in the global value block at +0x1D4C.
static double __fastcall MissionInfo_GetEliminationScore(uint32_t* self, void* /*edx*/) {
    uint8_t* globalBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));

    int32_t storedCount = CSF_AT(int32_t, self, 0x10);
    double eliminationValue = static_cast<double>(storedCount);
    if (storedCount < 0) {
        eliminationValue += static_cast<double>(g_UnsignedIntegerBias);
    }

    float eliminationScale = CSF_AT(float, globalBlock, 0x1D4C);
    return eliminationValue * eliminationScale;
}

HOOK(0x444D90, MissionInfo_GetEliminationScore, "auto");
