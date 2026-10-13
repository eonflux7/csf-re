#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_699DE0).
CSF_FUNC(int, __fastcall, sub_699DE0, 0x00699DE0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_0085e748: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_0085e748_Destroy(C_0085e748* self, void* /*edx*/, char flags)
{
    sub_699DE0(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x69A130, C_0085e748_Destroy, "auto");
