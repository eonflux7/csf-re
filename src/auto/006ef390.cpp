#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6EEF90).
CSF_FUNC(void, __fastcall, sub_6EEF90, 0x006EEF90, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00862110: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static int* __fastcall C_00862110_Destroy(C_00862110* self, void* /*edx*/, char flags)
{
    sub_6EEF90(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x6EF390, C_00862110_Destroy, "auto");
