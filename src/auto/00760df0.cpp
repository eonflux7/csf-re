#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_75FFE0).
CSF_FUNC(int, __fastcall, ReleaseBaseState, 0x0075FFE0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_0086c2b4: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void** __fastcall C_0086c2b4_Destroy(C_0086c2b4* self, void* /*edx*/, char flags)
{
    ReleaseBaseState(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<void**>(self);
}

HOOK(0x760DF0, C_0086c2b4_Destroy, "auto");
