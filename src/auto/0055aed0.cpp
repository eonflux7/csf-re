#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_55AE60).
CSF_FUNC(int, __fastcall, CleanupBaseObject, 0x0055AE60, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008449a0: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves it in eax.
static void* __fastcall C_008449a0_Destroy(C_008449a0* self, void* /*edx*/, char flags)
{
    CleanupBaseObject(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x55AED0, C_008449a0_Destroy, "auto");
