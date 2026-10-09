#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x0042DE60: fills two consecutive floats (x factor, y factor). Caller pops (cdecl).
CSF_FUNC(unsigned int, __cdecl, FillScaleFactorPair, 0x0042DE60, (float* a1));

// Scales the rectangle {left, top, right, bottom} in place. Position uses one factor pair,
// size uses a second factor pair drawn from the same callee. Returns the scaled height.
static int __cdecl Hud_ScaleRectPositionAndSize(int* rect)
{
    // Original geometry, read once on entry.
    const int originalLeft = rect[0];
    const int originalTop = rect[1];
    const int originalWidth = rect[2] - rect[0];
    const int originalHeight = rect[3] - rect[1];

    // Position scaling.
    float positionFactors[2] = {0.0f, 0.0f};
    FillScaleFactorPair(positionFactors);
    const int scaledLeft = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(originalLeft) * positionFactors[0]));
    const int scaledTop = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(originalTop) * positionFactors[1]));
    rect[0] = scaledLeft;
    rect[1] = scaledTop;

    // Size scaling, using a fresh factor pair.
    float sizeFactors[2] = {0.0f, 0.0f};
    FillScaleFactorPair(sizeFactors);
    const int scaledWidth = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(originalWidth) * sizeFactors[0]));
    const int scaledHeight = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(originalHeight) * sizeFactors[1]));

    // The original re-reads rect[0] and rect[1] after the second callee call.
    rect[2] = rect[0] + scaledWidth;
    rect[3] = rect[1] + scaledHeight;

    return scaledHeight;
}

HOOK(0x0042F440, Hud_ScaleRectPositionAndSize, "auto");
