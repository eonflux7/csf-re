#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

// Converts a box given as (corner A x,y,z, corner B x,y,z) into a bounding sphere:
// out[0..2] = center, out[3] = radius (half-diagonal length).
static float* __fastcall GfxBBoxMath_ComputeBoundingSphere(float* self, void* /*edx*/, float* outSphere) {
    const float halfScale = g_HalfDurationScale;

    // Half extents along each axis.
    const float halfX = (self[0] - self[3]) * halfScale;
    const float halfY = (self[1] - self[4]) * halfScale;
    const float halfZ = (self[2] - self[5]) * halfScale;

    // Center of the box.
    const float centerX = self[0] - halfX;
    const float centerY = self[1] - halfY;
    const float centerZ = self[2] - halfZ;

    // Radius = length of the half-diagonal.
    const float radius = std::sqrt(halfZ * halfZ + halfY * halfY + halfX * halfX);

    outSphere[0] = centerX;
    outSphere[1] = centerY;
    outSphere[2] = centerZ;
    outSphere[3] = radius;
    return outSphere;
}

HOOK(0x006AB7A0, GfxBBoxMath_ComputeBoundingSphere, "auto");
