#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_70B2D0).
CSF_FUNC(void, __fastcall, C_00860178_Cleanup, 0x0070B2D0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008636fc: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_008636fc_Destroy(C_008636fc* self, void* /*edx*/, char flags)
{
    C_00860178_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x70B620, C_008636fc_Destroy, "auto");
