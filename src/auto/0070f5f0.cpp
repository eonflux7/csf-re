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

// Releases the audio effect object's attached resource (field +0x50) when present.
CSF_FUNC(void, __cdecl, AudioEffect_Release, 0x007DF700, (int a1, int a2));

static int __cdecl AudioEffect_ReleaseIfAttached(int a1)
{
    void* effect = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    if (CSF_AT(int, effect, 0x50))
    {
        AudioEffect_Release(a1, 0);
    }
    return a1;
}

HOOK(0x0070F5F0, AudioEffect_ReleaseIfAttached, "auto");
