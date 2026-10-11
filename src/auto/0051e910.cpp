#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_51E890).
CSF_FUNC(int, __fastcall, sub_51E890, 0x0051E890, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00840f48: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does via mov eax, esi.
static void* __fastcall C_00840f48_Destroy(C_00840f48* self, void* /*edx*/, char flags)
{
    sub_51E890(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x51E910, C_00840f48_Destroy, "auto");
