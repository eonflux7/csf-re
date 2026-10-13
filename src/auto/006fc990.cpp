#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6FC270).
CSF_FUNC(void, __fastcall, sub_6FC270, 0x006FC270, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00862e88: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves it in eax.
static void* __fastcall C_00862e88_Destroy(C_00862e88* self, void* /*edx*/, char flags)
{
    sub_6FC270(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x6FC990, C_00862e88_Destroy, "auto");
