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

// Releases the audio effect's attached resource (field +0x50) through the lib helper at 0x007DF740.
CSF_FUNC(int, __cdecl, AudioEffect_ReleaseHandle, 0x007DF740, (int a1, int a2));

static int __cdecl AudioEffect_ReleaseAttachedObject(int a1)
{
    void* effect = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    if (CSF_AT(int, effect, 0x50))
    {
        AudioEffect_ReleaseHandle(a1, 0);
    }
    return a1;
}

HOOK(0x0070F610, AudioEffect_ReleaseAttachedObject, "auto");
