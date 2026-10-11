#include "hook.h"

// Quaternion product written in place: self = self * a2.
// Components are stored as [x, y, z, w] in indices 0..3.
// All inputs are read before any output is written.
static float* __fastcall GfxMatrix_MultiplyQuaternionInPlace(float* self, void* /*edx*/, float* a2)
{
    float* result = self;

    const float r0 = result[0];
    const float r1 = result[1];
    const float r2 = result[2];
    const float r3 = result[3];

    const float b0 = a2[0];
    const float b1 = a2[1];
    const float b2 = a2[2];
    const float b3 = a2[3];

    const float newX = b3 * r0 + r1 * b2 + r3 * b0 - r2 * b1;
    const float newY = r3 * b1 + b3 * r1 + r2 * b0 - r0 * b2;
    const float newZ = r3 * b2 + b3 * r2 + b1 * r0 - r1 * b0;
    const float newW = r3 * b3 - b0 * r0 - b1 * r1 - r2 * b2;

    result[0] = newX;
    result[1] = newY;
    result[2] = newZ;
    result[3] = newW;
    return result;
}

HOOK(0x6AE830, GfxMatrix_MultiplyQuaternionInPlace, "auto");
