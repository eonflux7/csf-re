#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6E6370).
CSF_FUNC(void, __fastcall, sub_6E6370, 0x006E6370, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00861ce0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00861ce0_Destroy(C_00861ce0* self, void* /*edx*/, char flags)
{
    sub_6E6370(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6E64A0, C_00861ce0_Destroy, "auto");
