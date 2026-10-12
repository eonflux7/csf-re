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

// Angle of a 2D vector in degrees, returned as double (fst0 result of the original).
CSF_FUNC(float, __cdecl, sub_6AEE40, 0x006AEE40, (float* a1, float* a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fEpsilon, 0x00830D20);
CSF_GLOBAL(float, g_fRadToDeg, 0x00830D18);

static double __stdcall Minimap_GetVectorAngleDegrees(float* vec)
{
    float y = vec[1];
    float x = vec[0];

    // Reference direction (0, -1), stored in the frame as the original does.
    float referenceDir[2];
    referenceDir[0] = 0.0f;
    referenceDir[1] = -1.0f;

    float magnitude = std::fabs(-std::sqrt(x * x + y * y));
    if (magnitude <= g_fEpsilon) {
        return static_cast<double>(g_fZero * g_fRadToDeg);
    }

    if (vec[0] < g_fZero) {
        float angle = sub_6AEE40(vec, referenceDir);
        return static_cast<double>(-angle * g_fRadToDeg);
    }

    float angle = sub_6AEE40(vec, referenceDir);
    return static_cast<double>(angle * g_fRadToDeg);
}

HOOK(0x0043B380, Minimap_GetVectorAngleDegrees, "auto");
