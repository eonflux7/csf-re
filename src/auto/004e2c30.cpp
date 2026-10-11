#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4E2C60).
CSF_FUNC(void, __fastcall, C_0082d410_BaseCleanup, 0x004E2C60, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083e500: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_0083e500_Destroy(C_0083e500* self, void* /*edx*/, char flags)
{
    C_0082d410_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4E2C30, C_0083e500_Destroy, "auto");
