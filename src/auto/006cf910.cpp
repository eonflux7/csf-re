#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_6CF860).
CSF_FUNC(void, __fastcall, C_00860c98_Cleanup, 0x006CF860, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00860cbc: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does (mov eax, esi).
static void* __fastcall C_00860cbc_Destroy(C_00860cbc* self, void* /*edx*/, char flags)
{
    C_00860c98_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6CF910, C_00860cbc_Destroy, "auto");
