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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_7707E0, 0x007707E0, (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);

// Returns 1 when no scene entry is attached to the guard's target, otherwise
// the result of the scene-entry test against the target position (a2 points
// to x, y, z floats; y is lifted by 50).
static char __fastcall S_004aa310_IsGuardStateValidForTarget(S_004aa310* self, void* /*edx*/, int a2)
{
    if (self->field_06ac == 0xFFFFFFFFu)
        return 1;

    uint8_t* gameState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* sceneLoader = CSF_AT(void*, gameState, 0x1FDC);

    int sceneEntryIndex = AiAttackTasks_FindSceneEntryIndex(sceneLoader, nullptr, self->field_06ac);
    if (sceneEntryIndex == -1)
        return 1;

    void* sceneEntry = reinterpret_cast<void*>(static_cast<uintptr_t>(
        SceneLoader_GetSceneEntry(sceneLoader, nullptr, sceneEntryIndex)));

    const float* targetPos = reinterpret_cast<const float*>(a2);
    float probePos[3];
    probePos[0] = targetPos[0];
    probePos[1] = targetPos[1] + g_fReturnedConstant_0082E030;
    probePos[2] = targetPos[2];

    return static_cast<char>(sub_7707E0(sceneEntry, nullptr, probePos));
}

HOOK(0x004AA310, S_004aa310_IsGuardStateValidForTarget, "auto");
