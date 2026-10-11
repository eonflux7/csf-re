#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4E5110).
CSF_FUNC(void, __fastcall, C_0083e160_BaseCleanup, 0x004E5110, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083e5e8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_0083e5e8_Destroy(C_0083e5e8* self, void* /*edx*/, char flags)
{
    C_0083e160_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4E2190, C_0083e5e8_Destroy, "auto");
