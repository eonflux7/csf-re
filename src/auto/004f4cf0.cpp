#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Cleanup step for C_0083f160 run before the object is released (0x4F4A80).
CSF_FUNC(int, __fastcall, C_0083f160_Destroy, 0x004F4A80, (C_0083f160* self, void*));

// Game-heap free, used only when the deleting flag is set (0x691110).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083f160: runs the base cleanup, then frees
// the object when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_0083f160_DeletingDestroy(C_0083f160* self, void* /*edx*/, char flags)
{
    C_0083f160_Destroy(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x4F4CF0, C_0083f160_DeletingDestroy, "auto");
