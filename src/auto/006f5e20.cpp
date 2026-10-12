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

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases a projection block: frees the owned sub-buffer at +0x0C (if any),
// clears that slot, then frees the block itself.
static int __stdcall GfxCameraProjection_Release(int a1)
{
    int result = 0;

    if (a1)
    {
        void* block = reinterpret_cast<void*>(a1);

        SceneLoader_NoOp();

        if (CSF_AT(uint32_t, block, 0x0C))
        {
            Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, block, 0x0C)));
            CSF_AT(uint32_t, block, 0x0C) = 0;
        }

        return Mem_Free(block);
    }

    return result;
}

HOOK(0x006F5E20, GfxCameraProjection_Release, "auto");
