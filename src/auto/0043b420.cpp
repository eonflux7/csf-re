#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_43ABF0).
CSF_FUNC(int, __fastcall, sub_43ABF0, 0x0043ABF0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00830d68: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves it in eax.
static int* __fastcall C_00830d68_Destroy(C_00830d68* self, void* /*edx*/, char flags)
{
    sub_43ABF0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x43B420, C_00830d68_Destroy, "auto");
