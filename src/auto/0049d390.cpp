#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_49D3C0).
CSF_FUNC(int, __fastcall, sub_49D3C0, 0x0049D3C0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00834050: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00834050_Destroy(C_00834050* self, void* /*edx*/, char flags)
{
    sub_49D3C0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x49D390, C_00834050_Destroy, "auto");
