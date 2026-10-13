#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step for C_00862028 (runs before the object is released).
CSF_FUNC(int, __fastcall, C_00862028_Destroy, 0x006ED110, (C_00862028* self, void*));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00862028: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00862028_DeletingDestroy(C_00862028* self, void* /*edx*/, char flags)
{
    C_00862028_Destroy(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x006EDE20, C_00862028_DeletingDestroy, "auto");
