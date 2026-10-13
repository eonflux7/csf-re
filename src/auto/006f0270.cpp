#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6F00F0).
CSF_FUNC(void, __fastcall, PTankBaseCleanup, 0x006F00F0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00862240: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00862240_Destroy(C_00862240* self, void* /*edx*/, char flags)
{
    PTankBaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x6F0270, C_00862240_Destroy, "auto");
