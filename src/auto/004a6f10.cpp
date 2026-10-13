#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (0x4DB3C0).
static CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

// Game heap free (0x691110).
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082d410: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static C_0082d410* __fastcall C_0082d410_Destroy(C_0082d410* self, void* /*edx*/, char flags)
{
    CharTaskBase_Init_4DB3C0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x004A6F10, C_0082d410_Destroy, "auto");
