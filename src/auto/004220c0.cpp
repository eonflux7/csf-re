#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_421620).
CSF_FUNC(int, __fastcall, sub_421620, 0x00421620, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082f9f8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static C_0082f9f8* __fastcall C_0082f9f8_Destroy(C_0082f9f8* self, void* /*edx*/, char flags)
{
    sub_421620(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4220C0, C_0082f9f8_Destroy, "auto");
