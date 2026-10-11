#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Cleanup step run before the profile manager is released (sub_515710).
CSF_FUNC(int, __fastcall, sub_515710, 0x00515710, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CProfileManager: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original does (mov eax, esi).
static void* __fastcall CProfileManager_DeletingDestroy(CProfileManager* self, void* /*edx*/, char flags)
{
    sub_515710(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x5156E0, CProfileManager_DeletingDestroy, "auto");
