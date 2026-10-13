#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6DF990).
CSF_FUNC(void, __fastcall, sub_6DF990, 0x006DF990, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00861988: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00861988_Destroy(C_00861988* self, void* /*edx*/, char flags)
{
    sub_6DF990(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x6E0080, C_00861988_Destroy, "auto");
