#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4E5D50).
CSF_FUNC(void, __fastcall, C_0082d410_Cleanup, 0x004E5D50, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083e9d0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_0083e9d0_Destroy(C_0083e9d0* self, void* /*edx*/, char flags)
{
    C_0082d410_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4E63E0, C_0083e9d0_Destroy, "auto");
