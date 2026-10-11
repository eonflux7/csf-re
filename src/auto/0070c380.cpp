#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_70C3B0).
CSF_FUNC(char, __fastcall, WaterShader_BaseCleanup, 0x0070C3B0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008637d8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves `this` in eax.
static void* __fastcall C_008637d8_Destroy(C_008637d8* self, void* /*edx*/, char flags)
{
    WaterShader_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x70C380, C_008637d8_Destroy, "auto");
