#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base cleanup step run before the object is released (sub_4063D0).
// Return value is not used by the caller, but the call must be kept.
CSF_FUNC(void, __fastcall, C_0082d410_CleanupBase, 0x004063D0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082d410: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static C_0082d410* __fastcall C_0082d410_Destroy(C_0082d410* self, void* /*edx*/, char flags)
{
    C_0082d410_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4063A0, C_0082d410_Destroy, "auto");
