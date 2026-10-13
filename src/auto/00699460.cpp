#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6993C0).
CSF_FUNC(int, __fastcall, CoreSortedTable_RunBaseCleanup, 0x006993C0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_0085e720: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the address of the object's vtable slot (the object itself), as the original does.
static void** __fastcall C_0085e720_Destroy(C_0085e720* self, void* /*edx*/, char flags)
{
    CoreSortedTable_RunBaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<void**>(self);
}

HOOK(0x699460, C_0085e720_Destroy, "auto");
