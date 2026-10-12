#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

static int __fastcall C_00830e5c_IsPointWithinRadiusOfMenuObject(C_00830e5c* self, void* /*edx*/, float* a2)
{
    void* sceneMenuContext = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    float radius = (float)self->vGetValue();
    float* objectPosition = GetObjectPositionVector(sceneMenuContext, nullptr);
    float dx = a2[0] - objectPosition[0];
    float dz = a2[2] - objectPosition[2];
    return (radius * radius >= dz * dz + dx * dx) ? 1 : 0;
}

HOOK(0x43AAD0, C_00830e5c_IsPointWithinRadiusOfMenuObject, "auto");
