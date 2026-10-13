#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Center of an axis-aligned box stored as two corner triples:
// self[0..2] and self[3..5] (min/max style). Writes the center to outCenter
// and returns outCenter.
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static float* __fastcall GfxBBox_GetCenter(float* self, void* /*edx*/, float* outCenter)
{
    float extentZ = self[2] - self[5];
    float halfExtentX = (self[0] - self[3]) * g_HalfDurationScale;
    float halfExtentY = (self[1] - self[4]) * g_HalfDurationScale;

    float centerZ = self[2] - extentZ * g_HalfDurationScale;
    float centerY = self[1] - halfExtentY;

    outCenter[0] = self[0] - halfExtentX;
    outCenter[1] = centerY;
    outCenter[2] = centerZ;
    return outCenter;
}

HOOK(0x006AB660, GfxBBox_GetCenter, "auto");
