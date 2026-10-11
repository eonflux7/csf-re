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

CSF_FUNC(int, __cdecl, RtAnimInterpolatorBlend, 0x0061F320, (int a1, int a2, int a3, int a4));

// Blends two HAnim interpolator nodes. pair[0] is the first node; pair[1] is the
// second node, whose +8 and +0xC children are the two source interpolators.
static int __stdcall HAnim_BlendInterpolatorPair(uint32_t* pair, int blendWeight)
{
    uint8_t* firstNode = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(pair[0]));
    uint8_t* secondNode = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(pair[1]));

    uint8_t* secondSourceA = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, secondNode, 0x8)));
    uint8_t* secondSourceB = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, secondNode, 0xC)));

    int firstValue = CSF_AT(int, firstNode, 0x20);
    int sourceAValue = CSF_AT(int, secondSourceA, 0x20);
    int sourceBValue = CSF_AT(int, secondSourceB, 0x20);

    return RtAnimInterpolatorBlend(firstValue, sourceAValue, sourceBValue, blendWeight);
}

HOOK(0x6E8D50, HAnim_BlendInterpolatorPair, "auto");
