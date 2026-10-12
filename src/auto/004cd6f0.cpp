#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step (TCharTask) run before the object is released (sub_4CD720).
CSF_FUNC(int, __fastcall, sub_4CD720, 0x004CD720, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083ab58: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_0083ab58_Destroy(C_0083ab58* self, void* /*edx*/, char flags)
{
    sub_4CD720(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4CD6F0, C_0083ab58_Destroy, "auto");
