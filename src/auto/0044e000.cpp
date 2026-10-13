#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_44DB40, base C_0082d410).
CSF_FUNC(int, __fastcall, C_0082d410_Cleanup, 0x0044DB40, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008311f0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_008311f0_Destroy(C_008311f0* self, void* /*edx*/, char flags)
{
    C_0082d410_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x44E000, C_008311f0_Destroy, "auto");
