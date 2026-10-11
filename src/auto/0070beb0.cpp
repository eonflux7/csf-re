#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(bool, __fastcall, GfxMatrix_RayIntersectsSphere, 0x006ACA40, (float* self, void*, float* sphere));
CSF_FUNC(int, __fastcall, GfxMatrix_IntersectRaySphere_6ACDF0, 0x006ACDF0, (float* self, void*, float* sphere, float* nearDistance, float* farDistance));
CSF_FUNC(int, __fastcall, sub_70BE30, 0x0070BE30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static bool __fastcall C_0086379c_IntersectScaledRay(C_0086379c* self, void* /*edx*/, float* ray, float* outDistance, float scale)
{
    alignas(8) uint8_t rayBytes[0x18];
    alignas(8) uint8_t sphereOutBytes[0x10];
    alignas(8) uint8_t boundBytes[0x34];
    float farDistance;

    float* rayOut = reinterpret_cast<float*>(rayBytes);
    uint32_t* bound = reinterpret_cast<uint32_t*>(boundBytes);

    // largest of the three radii/extents
    float* maxPtr = &self->field_001c;
    if (!(self->field_0018 < self->field_001c))
        maxPtr = &self->field_0018;
    if (self->field_0014 >= *maxPtr)
        maxPtr = &self->field_0014;

    bound[0] = self->field_0008;
    bound[1] = self->field_000c;
    bound[2] = self->field_0010;
    *reinterpret_cast<float*>(&bound[3]) = *maxPtr;

    if (!GfxMatrix_RayIntersectsSphere(ray, nullptr, reinterpret_cast<float*>(boundBytes)))
        return false;

    sub_70BE30(self, nullptr, reinterpret_cast<int>(sphereOutBytes));

    const double one = g_TransitionOne;
    double factor = (double)scale + one;

    // X: product and reciprocal are never stored to memory in the original
    double ix = one / (factor * (double)self->field_0014);

    // Y: product stored as float, reciprocal kept at full precision
    float sy = (float)(factor * (double)self->field_0018);
    double iy = one / (double)sy;

    // Z: product stored as float, reciprocal used both as double and as a float copy
    float sz = (float)(factor * (double)self->field_001c);
    double iz = one / (double)sz;
    float izFloat = (float)iz;

    rayOut[3] = (float)((double)ray[3] * ix);
    rayOut[4] = (float)((double)ray[4] * iy);
    rayOut[5] = (float)(iz * (double)ray[5]);
    rayOut[0] = (float)(ix * (double)ray[0]);
    rayOut[1] = (float)(iy * (double)ray[1]);
    rayOut[2] = (float)((double)izFloat * (double)ray[2]);

    NormalizeVector3(reinterpret_cast<C_00833b88*>(rayBytes), nullptr);

    int result = GfxMatrix_IntersectRaySphere_6ACDF0(rayOut, nullptr, reinterpret_cast<float*>(sphereOutBytes), outDistance, &farDistance);
    if (result == 1)
        *outDistance = 0.0f;
    return result != 0;
}

HOOK(0x0070BEB0, C_0086379c_IntersectScaledRay, "auto");
