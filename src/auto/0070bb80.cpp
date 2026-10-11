#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxMatrix_IntersectRaySphere_6ACDF0, 0x006ACDF0,
         (float* self, void* /*edx*/, float* sphere, float* nearDistance, float* farDistance));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Tests a ray (origin taken from the object's fields 0x08/0x0C/0x10, radius from field 0x14
// scaled by (a4 + 1.0)) against a sphere given by `a2`. Stores the entry distance in *a3 and
// clears it when the ray starts inside the sphere.
static bool __fastcall GfxWater_TestRayAgainstWaterSphere(C_00863760* self, void* /*edx*/,
                                                          float* a2, float* a3, float a4)
{
    uint32_t ray[4];
    ray[0] = self->field_0008;
    ray[1] = CSF_AT(uint32_t, self, 0x0C);
    ray[2] = CSF_AT(uint32_t, self, 0x10);

    float radiusScaled = (a4 + g_TransitionOne) * self->field_0014;
    std::memcpy(&ray[3], &radiusScaled, sizeof(float));

    int hitResult = GfxMatrix_IntersectRaySphere_6ACDF0(a2, nullptr,
                                                        reinterpret_cast<float*>(ray),
                                                        a3, &a4);
    if (hitResult == 1)
        *a3 = 0.0f;

    return hitResult != 0;
}

HOOK(0x0070BB80, GfxWater_TestRayAgainstWaterSphere, "auto");
