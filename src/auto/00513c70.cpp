#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_513CA0).
CSF_FUNC(int, __fastcall, sub_513CA0, 0x00513CA0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CSaveGameManager: runs the cleanup step, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does via the `this` register.
static void* __fastcall CSaveGameManager_Destroy(CSaveGameManager* self, void* /*edx*/, char flags)
{
    sub_513CA0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x513C70, CSaveGameManager_Destroy, "auto");
