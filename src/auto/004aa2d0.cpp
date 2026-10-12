#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets the object's vtable to C_0082d410's own vtable (the base-class step the original runs first).
CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0082d410: resets the vtable, then frees the object
// through the game heap when the low bit of the flags argument is set.
// Returns the object pointer, as the original leaves it in eax.
static C_0082d410* __fastcall C_0082d410_Destroy(C_0082d410* self, void* /*edx*/, char flags)
{
    C_0082d410_SetVtable(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x4AA2D0, C_0082d410_Destroy, "auto");
