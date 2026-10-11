#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_75E1E0).
CSF_FUNC(int, __fastcall, sub_75E1E0, 0x0075E1E0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0086c288: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_0086c288_Destroy(C_0086c288* self, void* /*edx*/, char flags)
{
    sub_75E1E0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x75E6E0, C_0086c288_Destroy, "auto");
