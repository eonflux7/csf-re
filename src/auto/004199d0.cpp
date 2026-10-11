#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_418AE0).
CSF_FUNC(int, __fastcall, C_0082de40_BaseCleanup, 0x00418AE0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082e1e8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_0082e1e8_Destroy(C_0082e1e8* self, void* /*edx*/, char flags)
{
    C_0082de40_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4199D0, C_0082e1e8_Destroy, "auto");
