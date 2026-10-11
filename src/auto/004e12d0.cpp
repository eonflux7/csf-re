#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the object's interface slot at +0x4 and primary vtable, then frees the
// block when bit 0 of the flags argument is set. Returns the object (eax = this).
static void* __fastcall C_0082d410_ResetAndReleaseIfFlagged(C_0082d410* self, void* /*edx*/, char releaseFlags)
{
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083E144);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x0082D410);

    if ((releaseFlags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x004E12D0, C_0082d410_ResetAndReleaseIfFlagged, "auto");
