#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_475790).
CSF_FUNC(int, __fastcall, CleanupBaseObject, 0x00475790, (void* self, void* edx));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00831698: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00831698_Destroy(C_00831698* self, void* /*edx*/, char flags)
{
    CleanupBaseObject(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return self;
}

HOOK(0x475980, C_00831698_Destroy, "auto");
