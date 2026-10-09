#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_499D40).
// Return value is not used by the destructor, but the call must be kept.
CSF_FUNC(int, __fastcall, CPlayerCharacter_CleanupBase, 0x00499D40, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor wrapper for CPlayerCharacter: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
static void* __fastcall CPlayerCharacter_Destroy(void* self, void* /*edx*/, char flags)
{
    CPlayerCharacter_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x49ABB0, CPlayerCharacter_Destroy, "auto");
