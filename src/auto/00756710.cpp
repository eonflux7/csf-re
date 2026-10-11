#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base cleanup step run before the object is released (sub_7563B0).
// Return value is not used by the destructor, but the call must be kept.
CSF_FUNC(unsigned int, __fastcall, C_00869cd0_CleanupBase, 0x007563B0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor wrapper for C_00869cd0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (its vtable slot sits at offset 0).
static void* __fastcall C_00869cd0_Destroy(void* self, void* /*edx*/, char flags)
{
    C_00869cd0_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x756710, C_00869cd0_Destroy, "auto");
