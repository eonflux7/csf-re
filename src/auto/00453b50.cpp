#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4A0FF0).
// Return value is not used by the destructor, but the call must be kept.
CSF_FUNC(int, __fastcall, Object_CleanupBase, 0x004A0FF0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor wrapper for C_00831278: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
static void* __fastcall C_00831278_Destroy(void* self, void* /*edx*/, char flags)
{
    Object_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x453B50, C_00831278_Destroy, "auto");
