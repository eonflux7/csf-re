#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6EA230).
CSF_FUNC(void, __fastcall, sub_6EA230, 0x006EA230, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00861e98: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the original returns `this` in eax).
static void* __fastcall C_00861e98_Destroy(C_00861e98* self, void* /*edx*/, char flags)
{
    sub_6EA230(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6EB3E0, C_00861e98_Destroy, "auto");
