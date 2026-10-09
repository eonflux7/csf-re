#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Scene-loader base cleanup step run before the object is released (sub_5035D0).
// The return value is not used, but the call must be kept.
CSF_FUNC(int, __fastcall, sub_5035D0, 0x005035D0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CSceneLoaderBase: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
static void* __fastcall CSceneLoaderBase_Destroy(void* self, void* /*edx*/, char flags)
{
    sub_5035D0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x503BC0, CSceneLoaderBase_Destroy, "auto");
