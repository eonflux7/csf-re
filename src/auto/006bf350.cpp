#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step for the embedded GFX state (sub_6BF180).
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00860700: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00860700_Destroy(C_00860700* self, void* /*edx*/, char flags)
{
    GfxObject_DestroyEmbeddedState(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6BF350, C_00860700_Destroy, "auto");
