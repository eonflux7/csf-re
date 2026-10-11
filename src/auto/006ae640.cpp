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

// Degrees-to-radians factor (pi / 180) stored in the game's data section.
CSF_GLOBAL(float, g_DegreesToRadians, 0x8600B0);

// Converts the Euler angles stored in self (x = yaw about Y, y = pitch about X)
// into a unit direction vector written to out (3 floats).
// Layout: self[0] = angle used for the horizontal plane, self[1] = elevation.
// Writes out[0] = x, out[1] = y (elevation sine, negated), out[2] = z.
static float* __fastcall GfxMatrix_EulerAnglesToDirection(float* self, void* /*edx*/, float* out)
{
    float elevationRadians = self[1] * g_DegreesToRadians;
    float elevationSine = -std::sin(elevationRadians);
    float elevationCosine = std::cos(elevationRadians);

    float azimuthRadians = self[0] * g_DegreesToRadians;
    float directionZ = std::cos(azimuthRadians) * elevationCosine;
    float directionX = std::sin(azimuthRadians) * elevationCosine;

    out[0] = directionX;
    out[1] = elevationSine;
    out[2] = directionZ;
    return out;
}

HOOK(0x006AE640, GfxMatrix_EulerAnglesToDirection, "auto");
