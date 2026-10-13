#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4DD140).
CSF_FUNC(int, __fastcall, C_0086c288_BaseCleanup, 0x004DD140, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083e0d8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_0083e0d8_Destroy(C_0083e0d8* self, void* /*edx*/, char flags)
{
    C_0086c288_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4DD4E0, C_0083e0d8_Destroy, "auto");
