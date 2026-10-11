#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

// Computes the max corner (at +0x08) and min corner (at +0x14) of a box centred on the
// scene bounds centre, from the full extents passed in.
static int __fastcall C_008637d8_SetBoxFromExtents(C_008637d8* self, void* /*edx*/, float* extents)
{
    float center[3];
    float* maxCorner = &CSF_AT(float, self, 0x8);
    float* minCorner = &CSF_AT(float, self, 0x14);

    SceneMissionSetup_GetBoundsCenter(reinterpret_cast<uint8_t*>(self) + 8, nullptr, center);

    float halfZ = extents[2] * g_HalfDurationScale;
    float halfY = extents[1] * g_HalfDurationScale;
    float halfX = extents[0] * g_HalfDurationScale;

    maxCorner[0] = halfX + center[0];
    maxCorner[1] = center[1] + halfY;
    maxCorner[2] = center[2] + halfZ;

    float halfX2 = extents[0] * g_HalfDurationScale;
    float halfY2 = extents[1] * g_HalfDurationScale;
    float halfZ2 = extents[2] * g_HalfDurationScale;

    minCorner[0] = center[0] - halfX2;
    float minY = center[1] - halfY2;
    minCorner[1] = minY;
    minCorner[2] = center[2] - halfZ2;

    int result;
    std::memcpy(&result, &minY, sizeof(result));
    return result;
}

HOOK(0x0070C660, C_008637d8_SetBoxFromExtents, "auto");
