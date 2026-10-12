#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Cleanup step run before the object is released (sub_559E90).
CSF_FUNC(int, __fastcall, sub_559E90, 0x00559E90, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00844928: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_00844928_Destroy(C_00844928* self, void* /*edx*/, char flags)
{
    sub_559E90(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x55A7D0, C_00844928_Destroy, "auto");
