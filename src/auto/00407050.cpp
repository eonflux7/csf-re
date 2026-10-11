#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_406BC0).
CSF_FUNC(int, __fastcall, C_0082d410_BaseCleanup, 0x00406BC0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082d77c: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer as int*, as the original returns `this` in eax.
static int* __fastcall C_0082d77c_Destroy(C_0082d77c* self, void* /*edx*/, char flags)
{
    C_0082d410_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x407050, C_0082d77c_Destroy, "auto");
