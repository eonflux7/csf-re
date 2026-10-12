#include <cstdint>
#include "hook.h"

// Decrements the counter at a2[0] when it is nonzero and returns a1.
// When the counter is zero, stores a1 in a2[1] and returns 0.
static int __cdecl GfxFrames_ConsumeCountOrSetPending(int a1, int32_t* a2)
{
    if (a2[0] != 0)
    {
        --a2[0];
        return a1;
    }

    a2[1] = a1;
    return 0;
}

HOOK(0x6BF2B0, GfxFrames_ConsumeCountOrSetPending, "auto");
