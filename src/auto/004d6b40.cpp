#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4D67A0).
CSF_FUNC(int, __fastcall, ReleaseCharTaskBaseState, 0x004D67A0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083c478: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_0083c478_Destroy(C_0083c478* self, void* /*edx*/, char flags)
{
    ReleaseCharTaskBaseState(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x4D6B40, C_0083c478_Destroy, "auto");
