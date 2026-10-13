#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ProximityMidDistanceSquared, 0x008339A8);
CSF_GLOBAL(float, g_WheelProcessingDistanceSquared, 0x0082E260);

static bool __stdcall AiDistracted_CheckLinkedActor(int a1)
{
    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (!sceneMenuContext)
        return false;

    float* selfPosition = GetObjectPositionVector(reinterpret_cast<void*>(static_cast<uintptr_t>(sceneMenuContext)), nullptr);

    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    int linkedBase = linkedObjectValue ? linkedObjectValue - 4 : 0;
    uint32_t linkedObject = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(linkedBase)), 0x2C);

    float* linkedPosition = GetObjectPositionVector(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObject)), nullptr);

    float dx = selfPosition[0] - linkedPosition[0];
    float dy = selfPosition[1] - linkedPosition[1];
    float dz = selfPosition[2] - linkedPosition[2];
    float distanceSquared = dx * dx + dz * dz + dy * dy;

    return distanceSquared >= g_ProximityMidDistanceSquared
        && distanceSquared <= g_WheelProcessingDistanceSquared;
}

HOOK(0x004ABEC0, AiDistracted_CheckLinkedActor, "auto");
