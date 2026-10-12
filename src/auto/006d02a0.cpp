#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6D02D0).
CSF_FUNC(void, __fastcall, C_00860da0_ReleaseSubobjects, 0x006D02D0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00860da0: runs the cleanup step, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00860da0_Destroy(C_00860da0* self, void* /*edx*/, char flags)
{
    C_00860da0_ReleaseSubobjects(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6D02A0, C_00860da0_Destroy, "auto");
