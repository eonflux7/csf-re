#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original heap free used by the game for this allocation.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets the object's owned buffer: frees the block at +0x08 if present,
// then clears the three pointer/size fields. Returns the free result,
// or the (zero) buffer pointer when nothing was freed.
static int __fastcall C_0086a3c0_ReleaseBufferAndReset(C_0086a3c0* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0086A3C0);

    int result = static_cast<int>(self->field_0008);
    if (self->field_0008) {
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    return result;
}

HOOK(0x007567E0, C_0086a3c0_ReleaseBufferAndReset, "auto");
