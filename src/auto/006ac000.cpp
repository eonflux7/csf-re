#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, S_006abb80_GetCornerPoints, 0x006ABC40,
         (void* self, void* edx, float* a2));

static void __fastcall S_006abb80_ComputeAxisAlignedBounds(
    S_006abb80* self, void* /*edx*/,
    float* maxX, float* minX,
    float* maxY, float* minY,
    float* maxZ, float* minZ)
{
    float corners[8][3] = {};
    S_006abb80_GetCornerPoints(self, nullptr, &corners[0][0]);

    *maxX = corners[0][0];
    *minX = corners[0][0];
    *maxY = corners[0][1];
    *minY = corners[0][1];
    *maxZ = corners[0][2];
    *minZ = corners[0][2];

    for (int i = 1; i < 8; ++i)
    {
        if (corners[i][0] > *maxX)
            *maxX = corners[i][0];
        if (corners[i][0] < *minX)
            *minX = corners[i][0];

        if (corners[i][1] > *maxY)
            *maxY = corners[i][1];
        if (corners[i][1] < *minY)
            *minY = corners[i][1];

        if (corners[i][2] > *maxZ)
            *maxZ = corners[i][2];
        if (corners[i][2] < *minZ)
            *minZ = corners[i][2];
    }
}

HOOK(0x006AC000, S_006abb80_ComputeAxisAlignedBounds, "auto");
