#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step (C_0082d410) run before the object is released.
CSF_FUNC(int, __fastcall, C_0082d410_BaseCleanup, 0x004F7780, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083f1a0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static C_0083f1a0* __fastcall C_0083f1a0_Destroy(C_0083f1a0* self, void* /*edx*/, char flags)
{
    C_0082d410_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4F8120, C_0083f1a0_Destroy, "auto");
