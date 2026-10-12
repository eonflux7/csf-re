#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (C_008629f8_Destroy at 0x6F90D0).
CSF_FUNC(void, __fastcall, C_008629f8_Destroy, 0x006F90D0, (C_008629f8* self, void* edx));

// Game heap free (Mem_Free at 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_008629f8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_008629f8_DeletingDestructor(C_008629f8* self, void* /*edx*/, char flags)
{
    C_008629f8_Destroy(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6F90A0, C_008629f8_DeletingDestructor, "auto");
