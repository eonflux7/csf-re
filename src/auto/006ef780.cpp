#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6EF720).
CSF_FUNC(void, __fastcall, ReleaseBaseResources, 0x006EF720, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_008621c8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_008621c8_Destroy(C_008621c8* self, void* /*edx*/, char flags)
{
    ReleaseBaseResources(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x6EF780, C_008621c8_Destroy, "auto");
