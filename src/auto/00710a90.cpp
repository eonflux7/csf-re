#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_70F2D0).
CSF_FUNC(int, __fastcall, C_00863800_BaseCleanup, 0x0070F2D0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00863ba8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the original leaves `this` in eax).
static void* __fastcall C_00863ba8_Destroy(C_00863ba8* self, void* /*edx*/, char flags)
{
    C_00863800_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x710A90, C_00863ba8_Destroy, "auto");
