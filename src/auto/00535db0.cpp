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

// Game heap free: cdecl, one argument, pops nothing itself.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the scene-result instancing buffers: frees the 0x14C block with the game
// heap, then calls slot 0 of the object at 0x13C with arg 1 (its destructor-style
// release) and clears both pointers.
static int __fastcall SceneResultInstancing_ReleaseBuffers(void* self, void* /*edx*/)
{
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x13C));
    if (result)
    {
        result = static_cast<int>(CSF_AT(uint32_t, self, 0x14C));
        if (result)
        {
            result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x14C))));
            CSF_AT(uint32_t, self, 0x14C) = 0;
        }

        void* releasable = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x13C)));
        if (releasable)
        {
            result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, releasable, 0)[0])(releasable, nullptr, 1);
            CSF_AT(uint32_t, self, 0x13C) = 0;
        }
    }
    return result;
}

HOOK(0x00535DB0, SceneResultInstancing_ReleaseBuffers, "auto");
