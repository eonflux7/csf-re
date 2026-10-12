#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_40BCB0).
CSF_FUNC(int, __fastcall, sub_40BCB0, 0x0040BCB0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082daf4: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (left in eax by the original).
static void* __fastcall C_0082daf4_Destroy(C_0082daf4* self, void* /*edx*/, char flags)
{
    sub_40BCB0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x40C0E0, C_0082daf4_Destroy, "auto");
