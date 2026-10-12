#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the buffer held at +0x0C (if any), then frees the object itself
// when bit 0 of the argument is set. Returns the object.
static uint32_t* __fastcall ReleaseOwnedBlock(uint32_t* self, void* /*edx*/, char releaseSelf)
{
    SceneLoader_NoOp();
    if (CSF_AT(uint32_t, self, 0x0C) != 0)
    {
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C)));
        CSF_AT(uint32_t, self, 0x0C) = 0;
    }
    if ((releaseSelf & 1) != 0)
    {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x006DA380, ReleaseOwnedBlock, "auto");
