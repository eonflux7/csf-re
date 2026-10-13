#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4A8680).
CSF_FUNC(int, __fastcall, TCharTask_BaseCleanup, 0x004A8680, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00834fa8 (TIrAPuesto/TSubido/TIrASalida/TBajar/TSubir/TSentarse family):
// runs the base TCharTask cleanup, then frees the object through the game heap
// when the low bit of the flags argument is set. Returns the object pointer.
static void* __fastcall C_00834fa8_Destroy(C_00834fa8* self, void* /*edx*/, char flags)
{
    TCharTask_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4A8650, C_00834fa8_Destroy, "auto");
