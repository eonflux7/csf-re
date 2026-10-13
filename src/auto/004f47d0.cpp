#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4F46E0).
CSF_FUNC(int, __fastcall, sub_4F46E0, 0x004F46E0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_0083f130: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the original returns &this->__vftable, i.e. the object address).
static void* __fastcall C_0083f130_Destroy(C_0083f130* self, void* /*edx*/, char flags)
{
    sub_4F46E0(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x4F47D0, C_0083f130_Destroy, "auto");
