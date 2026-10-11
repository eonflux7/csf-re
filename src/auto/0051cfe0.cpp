#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(float, g_ProximityMidDistanceSquared, 0x008339A8);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __stdcall FxParticleManager_IsWithinActiveSceneRange(C_008342e8* object)
{
    if (!object)
        return false;
    if (!object->field_0050)
        return true;

    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        return false;

    auto* sceneContext = reinterpret_cast<C_008342e8*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!sceneContext)
        return false;

    float* objectPosition = GetObjectPositionVector(object, nullptr);
    float* scenePosition = GetObjectPositionVector(sceneContext, nullptr);

    const float dx = CSF_AT(float, scenePosition, 0x0)
                   - CSF_AT(float, objectPosition, 0x0);
    const float dy = CSF_AT(float, scenePosition, 0x4)
                   - CSF_AT(float, objectPosition, 0x4);
    const float dz = CSF_AT(float, scenePosition, 0x8)
                   - CSF_AT(float, objectPosition, 0x8);

    return dz * dz + dx * dx + dy * dy < g_ProximityMidDistanceSquared;
}

HOOK(0x0051CFE0, FxParticleManager_IsWithinActiveSceneRange, "auto");
