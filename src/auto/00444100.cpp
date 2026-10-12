#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_443F00).
CSF_FUNC(int, __fastcall, sub_443F00, 0x00443F00, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083109c: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static void* __fastcall C_0083109c_Destroy(C_0083109c* self, void* /*edx*/, char flags)
{
    sub_443F00(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x444100, C_0083109c_Destroy, "auto");
