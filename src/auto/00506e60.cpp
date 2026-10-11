#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_506E90).
CSF_FUNC(int, __fastcall, sub_506E90, 0x00506E90, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CNetProfile: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall CNetProfile_DestroyAndMaybeFree(CNetProfile* self, void* /*edx*/, char flags)
{
    sub_506E90(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x506E60, CNetProfile_DestroyAndMaybeFree, "auto");
