#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_523870).
// Return value is not used by the destructor, but the call must be kept.
CSF_FUNC(int, __fastcall, Audio_CleanupBase, 0x00523870, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor wrapper for C_00840ff0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
static void* __fastcall C_00840ff0_Destroy(void* self, void* /*edx*/, char flags)
{
    Audio_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x523840, C_00840ff0_Destroy, "auto");
