#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step (C_0083f4bc) run before the object is released (sub_691AC0).
CSF_FUNC(int, __fastcall, C_0083f4bc_Cleanup, 0x00691AC0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0085e5c4: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_0085e5c4_Destroy(C_0085e5c4* self, void* /*edx*/, char flags)
{
    C_0083f4bc_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6920F0, C_0085e5c4_Destroy, "auto");
