#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Water state finalizer, run before the object is released.
CSF_FUNC(char, __fastcall, Water_FinalizeState, 0x006FE010, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for S_006fdfe0: finalizes the water state, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall S_006fdfe0_Destroy(S_006fdfe0* self, void* /*edx*/, char flags)
{
    Water_FinalizeState(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6FDFE0, S_006fdfe0_Destroy, "auto");
