#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_459060).
CSF_FUNC(void, __fastcall, ReleaseIconStatusBase, 0x00459060, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008313dc: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves it in eax.
static C_008313dc* __fastcall C_008313dc_Destroy(C_008313dc* self, void* /*edx*/, char flags)
{
    ReleaseIconStatusBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x459570, C_008313dc_Destroy, "auto");
