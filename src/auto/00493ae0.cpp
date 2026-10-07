#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall ToggleFlagMask(void* self, void* /*edx*/, int mask)
{
    const int flags = CSF_AT(int, self, 0x2E4);
    if ((flags & mask) == mask)
    {
        const int remainingFlags = flags & ~mask;
        CSF_AT(int, self, 0x2E4) = remainingFlags;
        return remainingFlags;
    }

    CSF_AT(int, self, 0x2E4) = flags | mask;
    return mask;
}

HOOK(0x00493AE0, ToggleFlagMask, "auto");
