#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6C7080).
CSF_FUNC(void, __fastcall, sub_6C7080, 0x006C7080, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008608e8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_008608e8_Destroy(C_008608e8* self, void* /*edx*/, char flags)
{
    sub_6C7080(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6C72A0, C_008608e8_Destroy, "auto");
