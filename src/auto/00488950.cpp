#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step for CCharWeaponFsm, run before the object is released (sub_487A40).
// Return value is not used by the destructor, but the call must be kept.
CSF_FUNC(int, __fastcall, CCharWeaponFsm_ReleaseState, 0x00487A40, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CCharActionFsm: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
static void* __fastcall CCharActionFsm_Destroy(void* self, void* /*edx*/, char flags)
{
    CCharWeaponFsm_ReleaseState(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x488950, CCharActionFsm_Destroy, "auto");
