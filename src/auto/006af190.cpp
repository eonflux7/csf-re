#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the embedded member object before the owner is destroyed (sub_6AEEC0).
CSF_FUNC(int, __fastcall, ReleaseEmbeddedMember_6AEEC0, 0x006AEEC0, (void* self, void* edx));

// Game heap free (Mem_Free).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008600d4: releases the embedded member, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_008600d4_Destroy(C_008600d4* self, void* /*edx*/, char flags)
{
    ReleaseEmbeddedMember_6AEEC0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6AF190, C_008600d4_Destroy, "auto");
