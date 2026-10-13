#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (C_00860178 side of the hierarchy).
CSF_FUNC(void, __fastcall, C_00860388_DestroyBase, 0x006B7F00, (void* self, void* edx));

// Frees a block allocated by the game's core heap.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00860388: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00860388_Destroy(C_00860388* self, void* /*edx*/, char flags)
{
    C_00860388_DestroyBase(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x6B7FF0, C_00860388_Destroy, "auto");
