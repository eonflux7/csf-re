#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, S_006abb80_SetBounds, 0x006ABB80,
         (void* self, void* edx, float a2, float a3, float a4,
          float a5, float a6, float a7));
CSF_FUNC(void, __fastcall, S_006abb80_GetBounds, 0x006AC000,
         (void* self, void* edx, float* a2, float* a3, float* a4,
          float* a5, float* a6, float* a7));

static float* __fastcall S_006abb80_ExpandBounds(
    S_006abb80* self, void* /*edx*/, S_006abb80* other)
{
    // The packet also identifies the SetBounds address as a vtable reference.
    (void)CSF_VTBL(0x006ABB80);

    float maxX, minX, maxY, minY, maxZ, minZ;
    float otherMaxX, otherMinX, otherMaxY, otherMinY, otherMaxZ, otherMinZ;

    S_006abb80_GetBounds(self, nullptr,
                        &maxX, &minX, &maxY, &minY, &maxZ, &minZ);
    S_006abb80_GetBounds(other, nullptr,
                        &otherMaxX, &otherMinX, &otherMaxY, &otherMinY,
                        &otherMaxZ, &otherMinZ);

    if (otherMaxX > maxX)
        maxX = otherMaxX;
    if (otherMaxY > maxY)
        maxY = otherMaxY;
    if (otherMaxZ > maxZ)
        maxZ = otherMaxZ;
    if (otherMinX < minX)
        minX = otherMinX;
    if (otherMinY < minY)
        minY = otherMinY;
    if (otherMinZ < minZ)
        minZ = otherMinZ;

    S_006abb80_SetBounds(self, nullptr, maxX, minX, maxY, minY, maxZ, minZ);
    return &CSF_AT(float, self, 0x00);
}

HOOK(0x006AC5E0, S_006abb80_ExpandBounds, "auto");
