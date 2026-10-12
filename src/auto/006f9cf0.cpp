#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base cleanup for C_00862b58, run before the object is released.
CSF_FUNC(void, __fastcall, C_00862b58_Destroy, 0x006F9D20, (C_00862b58* self, void* edx));

// Game heap free (Mem_Free).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00862b58: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00862b58_DeletingDestroy(C_00862b58* self, void* /*edx*/, char flags)
{
    C_00862b58_Destroy(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x006F9CF0, C_00862b58_DeletingDestroy, "auto");
