#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class destructor step run before the object is released (0x4920F0).
CSF_FUNC(int, __fastcall, C_00833b88_Destructor, 0x004920F0, (void* self, void* edx));

// Game heap free (0x691110), used by the deleting path.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00833b88: runs the destructor body, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the original returns &this->__vftable, which is the object address).
static void* __fastcall C_00833b88_Destroy(C_00833b88* self, void* /*edx*/, char flags)
{
    C_00833b88_Destructor(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x492C50, C_00833b88_Destroy, "auto");
