#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCandidateRadiusQuery, 0x00443770, (void* self, void* edx, float a2, int a3));

// Checks whether a candidate position (taken from the sub-object at +0x38) has no
// conflicting assignment within a 30.0 radius query on the current scene's AI manager.
// Returns true when the query reports no hit (result == 0). Always bumps the per-frame search cost.
static bool __fastcall S_004abb90_IsCandidateUnassigned(S_004abb90* self, void* /*edx*/, int /*a2*/)
{
    uint32_t* positionData = AiCoverAttackTasks_GetPositionData(
        reinterpret_cast<uint8_t*>(self) + 0x38, nullptr);

    int queryVec[5];
    queryVec[0] = static_cast<int>(positionData[0]);
    queryVec[1] = static_cast<int>(positionData[1]);
    queryVec[2] = static_cast<int>(positionData[2]);
    queryVec[3] = 0x42DC0000; // 110.0f
    queryVec[4] = 0x43480000; // 200.0f

    void* searchOwner = CSF_AT(void*, g_pCurrentScene, 0xB8);
    int queryResult = AiCandidateRadiusQuery(
        searchOwner, nullptr, 30.0f,
        static_cast<int>(reinterpret_cast<uintptr_t>(queryVec)));

    ++g_AiSearchCostThisFrame;
    return queryResult == 0;
}

HOOK(0x004ABC10, S_004abb90_IsCandidateUnassigned, "auto");
