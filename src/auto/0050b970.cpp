#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_50B8C0).
CSF_FUNC(int, __fastcall, sub_50B8C0, 0x0050B8C0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for COptionsFile: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall COptionsFile_Destroy(COptionsFile* self, void* /*edx*/, char flags)
{
    sub_50B8C0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x50B970, COptionsFile_Destroy, "auto");
