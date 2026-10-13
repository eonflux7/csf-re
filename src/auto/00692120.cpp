#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (0x691D20, base C_0083f4bc).
CSF_FUNC(int, __fastcall, C_0083f4bc_CleanupStep, 0x00691D20, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0085e5d8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the original returns this in eax).
static int* __fastcall C_0085e5d8_Destroy(C_0085e5d8* self, void* /*edx*/, char flags)
{
    C_0083f4bc_CleanupStep(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x692120, C_0085e5d8_Destroy, "auto");
