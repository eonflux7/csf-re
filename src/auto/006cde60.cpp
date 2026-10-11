#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6CDCF0 on C_00860af0 base).
CSF_FUNC(void, __fastcall, sub_6CDCF0, 0x006CDCF0, (void* self, void* edx));

// Game heap free (Mem_Alloc/Mem_Free pair).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00860bb8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00860bb8_Destroy(C_00860bb8* self, void* /*edx*/, char flags)
{
    sub_6CDCF0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6CDE60, C_00860bb8_Destroy, "auto");
