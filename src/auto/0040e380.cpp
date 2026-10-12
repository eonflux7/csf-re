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

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HourTimeFactor, 0x0082DD70);
CSF_GLOBAL(float, g_SpeedUnitScale, 0x0082DD6C);

// Returns the Euclidean length of the vector stored at self+0x64..0x6C, divided by
// the global speed scale (field 0x1D70 of the global block) times the constants.
static float __fastcall GetScaledSpeedMagnitude(float* self, void* /*edx*/)
{
    float x = CSF_AT(float, self, 0x64);
    float y = CSF_AT(float, self, 0x68);
    float z = CSF_AT(float, self, 0x6C);

    float sumSquares = x * x + y * y + z * z;

    uint8_t* globalBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    float globalSpeedScale = CSF_AT(float, globalBlock, 0x1D70);

    return std::sqrt(sumSquares) / (globalSpeedScale * g_HourTimeFactor * g_SpeedUnitScale);
}

HOOK(0x0040E380, GetScaledSpeedMagnitude, "auto");
