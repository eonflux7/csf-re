#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (C_0083b958 destructor body).
CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (C_0083b958* self, void*));

// Frees a block through the game's core memory allocator.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting-destructor style entry for C_00835060: runs the base cleanup, then frees
// the object through the core allocator when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static void* __fastcall C_00835060_HandleSubobjectRequest(C_00835060* self, void* /*edx*/, char flags)
{
    C_0083b958_Destroy(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x4A8B90, C_00835060_HandleSubobjectRequest, "auto");
