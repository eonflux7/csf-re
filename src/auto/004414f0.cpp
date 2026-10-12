#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_441160).
CSF_FUNC(int, __fastcall, sub_441160, 0x00441160, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00831008: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static void* __fastcall C_00831008_Destroy(C_00831008* self, void* /*edx*/, char flags)
{
    sub_441160(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4414F0, C_00831008_Destroy, "auto");
