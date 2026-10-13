#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4A9BC0).
CSF_FUNC(int, __fastcall, sub_4A9BC0, 0x004A9BC0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00835228: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00835228_Destroy(C_00835228* self, void* /*edx*/, char flags)
{
    sub_4A9BC0(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x4A9B90, C_00835228_Destroy, "auto");
