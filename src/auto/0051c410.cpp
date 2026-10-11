#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_51C310).
CSF_FUNC(void, __fastcall, sub_51C310, 0x0051C310, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00840ec0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_00840ec0_Destroy(C_00840ec0* self, void* /*edx*/, char flags)
{
    sub_51C310(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x51C410, C_00840ec0_Destroy, "auto");
