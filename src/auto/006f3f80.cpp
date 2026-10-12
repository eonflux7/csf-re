#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000,
         (S_006abb80* self, void* /*edx*/, float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));

// Writes the min corner into minOut (x, y, z) and the max corner into maxOut (x, y, z).
// Returns maxOut, matching the original's return of arg4.
static float* __fastcall RpPrtStdAtomic_GetBoundsMinMax(void* self, void* /*edx*/, float* minOut, float* maxOut)
{
    float maxX, minX, maxY, minY, maxZ, minZ;

    S_006abb80_ComputeAxisAlignedBounds(
        reinterpret_cast<S_006abb80*>(static_cast<uint8_t*>(self) + 0x24), nullptr,
        &maxX, &minX, &maxY, &minY, &maxZ, &minZ);

    minOut[0] = minX;
    minOut[1] = minY;
    minOut[2] = minZ;

    maxOut[0] = maxX;
    maxOut[1] = maxY;
    maxOut[2] = maxZ;

    return maxOut;
}

HOOK(0x006F3F80, RpPrtStdAtomic_GetBoundsMinMax, "auto");
