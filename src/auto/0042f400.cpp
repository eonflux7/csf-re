#include "hook.h"
#include "globals.h"

// Fills a two-float scale pair (x, y) for the loading menu. The original's
// name is unknown, so this name describes what the callee does with the buffer.
CSF_FUNC(unsigned int, __cdecl, FillLoadingScalePair, 0x0042DE60, (float* a1));

static float* __cdecl MenuLoading_AdjustFloatPair(float* a1)
{
    float scale[2];
    scale[0] = 0.0f;
    scale[1] = 0.0f;

    FillLoadingScalePair(scale);

    a1[0] = scale[0] * a1[0];
    a1[1] = scale[1] * a1[1];
    return a1;
}

HOOK(0x0042F400, MenuLoading_AdjustFloatPair, "auto");
