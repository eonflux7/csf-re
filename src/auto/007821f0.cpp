#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base cleanup for CLevelData: releases the level-data resources before the object is freed.
CSF_FUNC(int, __fastcall, sub_781CF0, 0x00781CF0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CLevelData: runs the resource cleanup, then frees the
// object through the game heap when the low bit of the flags argument is set.
static void* __fastcall CLevelData_Destroy(void* self, void* /*edx*/, char flags)
{
    sub_781CF0(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x007821F0, CLevelData_Destroy, "auto");
