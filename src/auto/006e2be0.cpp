#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the emitter object is released (sub_6E2C10).
CSF_FUNC(void, __fastcall, ParticleEmitterBase_Cleanup, 0x006E2C10, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00861a24: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00861a24_Destroy(C_00861a24* self, void* /*edx*/, char flags)
{
    ParticleEmitterBase_Cleanup(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x6E2BE0, C_00861a24_Destroy, "auto");
