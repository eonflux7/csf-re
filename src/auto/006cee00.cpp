#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6CED00).
CSF_FUNC(void, __fastcall, sub_6CED00, 0x006CED00, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00860c88: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves it in eax.
static int* __fastcall C_00860c88_Destroy(C_00860c88* self, void* /*edx*/, char flags)
{
    sub_6CED00(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x6CEE00, C_00860c88_Destroy, "auto");
