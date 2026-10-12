#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step (C_00860170 teardown) run before the object is released.
CSF_FUNC(void, __fastcall, C_008629a4_Destroy, 0x006F8FF0, (C_008629a4* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008629a4: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the original returns `this` in eax).
static void* __fastcall C_008629a4_DeletingDestroy(C_008629a4* self, void* /*edx*/, char flags)
{
    C_008629a4_Destroy(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6F9060, C_008629a4_DeletingDestroy, "auto");
