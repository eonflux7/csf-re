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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, GetSceneChildForTarget, 0x004DE7A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(bool, __fastcall, ActorTargetMatchesPoint, 0x004E0BF0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, CandidateTargetAccepts, 0x0041E350, (void* a1, void* a2));

// Returns the cached target reference stored at this+0x234, re-resolving it from the
// current scene when the cache is empty or no longer matches the requested target.
static void* __fastcall AITask_ResolveTargetRef(S_004e0a70* self, void* /*edx*/, S_004e0a70* a2)
{
    void* cachedTarget = CSF_AT(void*, self, 0x234);

    if (cachedTarget == nullptr ||
        !ActorTargetMatchesPoint(cachedTarget, nullptr, reinterpret_cast<float*>(a2)))
    {
        uint32_t sceneChild = CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);

        int resolvedTarget = GetSceneChildForTarget(
            reinterpret_cast<void*>(sceneChild), nullptr,
            reinterpret_cast<unsigned int>(a2));

        CSF_AT(void*, self, 0x234) = reinterpret_cast<void*>(resolvedTarget);

        if (!CandidateTargetAccepts(reinterpret_cast<void*>(resolvedTarget), a2))
            CSF_AT(void*, self, 0x234) = nullptr;
    }

    return CSF_AT(void*, self, 0x234);
}

HOOK(0x00420B40, AITask_ResolveTargetRef, "auto");
