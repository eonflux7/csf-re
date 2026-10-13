#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (Tables_ReleaseBaseState).
CSF_FUNC(void, __fastcall, Tables_ReleaseBaseState, 0x0078C8E0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00872abc: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static void* __fastcall C_00872abc_Destroy(C_00872abc* self, void* /*edx*/, char flags)
{
    Tables_ReleaseBaseState(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x78C9D0, C_00872abc_Destroy, "auto");
