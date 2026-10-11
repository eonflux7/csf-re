#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_412000).
// Return value is not used by the caller, but the call must be kept.
CSF_FUNC(int, __fastcall, Object_CleanupBase, 0x00412000, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082dfd0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_0082dfd0_Destroy(C_0082dfd0* self, void* /*edx*/, char flags)
{
    Object_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x412AE0, C_0082dfd0_Destroy, "auto");
