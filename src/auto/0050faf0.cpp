#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_50F540).
CSF_FUNC(int*, __fastcall, CMenuManager_BaseCleanup, 0x0050F540, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CMenuManager: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does (mov eax, esi).
static void* __fastcall CMenuManager_Destroy(void* self, void* /*edx*/, char flags)
{
    CMenuManager_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x50FAF0, CMenuManager_Destroy, "auto");
