#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_4734D0).
// Return value is not used by the destructor, but the call must be kept.
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x4734D0,
         (char** self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor wrapper for C_008313f8: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
static void* __fastcall C_008313f8_Destroy(void* self, void* /*edx*/, char flags)
{
    Object_PrepareForDestruction(reinterpret_cast<char**>(self), nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x45BC60, C_008313f8_Destroy, "auto");
