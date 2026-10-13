#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_49F4B0).
CSF_FUNC(void, __fastcall, C_00833908_ReleaseBaseResources, 0x0049F4B0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_008342e8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_008342e8_Destroy(C_008342e8* self, void* /*edx*/, char flags)
{
    C_00833908_ReleaseBaseResources(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x0049F530, C_008342e8_Destroy, "auto");
