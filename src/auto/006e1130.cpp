#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6E1160).
CSF_FUNC(void, __fastcall, C_00861a28_BaseCleanup, 0x006E1160, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00861a28: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00861a28_Destroy(C_00861a28* self, void* /*edx*/, char flags)
{
    C_00861a28_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6E1130, C_00861a28_Destroy, "auto");
