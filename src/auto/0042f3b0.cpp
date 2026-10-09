// Module: hud. Scales a two-dword HUD size record (width, height) by the factors
// returned from sub_42DE60, storing the truncated 32-bit results back in place.
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

// Fills two float scale factors (x, y) at the given pointer.
CSF_FUNC(unsigned int, __cdecl, ComputeHudScaleFactors, 0x0042DE60, (float* a1));

static uint64_t __cdecl ScaleHudSizePair(int* sizePair)
{
    float scaleFactors[2];
    scaleFactors[0] = 0.0f;
    scaleFactors[1] = 0.0f;

    ComputeHudScaleFactors(scaleFactors);

    // Width: read, scale, truncate to 32 bits, store.
    double scaledWidth = static_cast<double>(sizePair[0]) * scaleFactors[0];
    int32_t newWidth = static_cast<int32_t>(static_cast<int64_t>(scaledWidth));
    sizePair[0] = newWidth;

    // Height: read (after the width store, as in the original), scale, truncate, store.
    double scaledHeight = static_cast<double>(sizePair[1]) * scaleFactors[1];
    int32_t newHeight = static_cast<int32_t>(static_cast<int64_t>(scaledHeight));
    sizePair[1] = newHeight;

    // The original returns the last value left in eax (low dword of the height result).
    return static_cast<uint64_t>(static_cast<uint32_t>(newHeight));
}

HOOK(0x42F3B0, ScaleHudSizePair, "auto");
