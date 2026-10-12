#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6D2520).
CSF_FUNC(void, __fastcall, C_008631c8_ReleaseBaseState, 0x006D2520, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00860e68: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00860e68_Destroy(C_00860e68* self, void* /*edx*/, char flags)
{
    C_008631c8_ReleaseBaseState(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6D2610, C_00860e68_Destroy, "auto");
