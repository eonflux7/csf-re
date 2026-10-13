#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released.
CSF_FUNC(int, __fastcall, DestroyBaseLayer, 0x00771030, (void* self, void* edx));

// Frees a block through the game heap.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00871914: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static void* __fastcall C_00871914_Destroy(C_00871914* self, void* /*edx*/, char flags)
{
    DestroyBaseLayer(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x7711E0, C_00871914_Destroy, "auto");
