#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step (C_0082d410) run before the object is released (sub_4A4030).
CSF_FUNC(int, __fastcall, sub_4A4030, 0x004A4030, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008344f0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_008344f0_Destroy(C_008344f0* self, void* /*edx*/, char flags)
{
    sub_4A4030(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4A40B0, C_008344f0_Destroy, "auto");
