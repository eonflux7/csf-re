#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Scalar deleting destructor for C_008619e0.
// Resets the vtable, clears the byte flag at +0x8 if it was set, and
// optionally frees the object through the game's memory block free.

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static C_008619e0* __fastcall C_008619e0_Destroy(C_008619e0* self, void* /*edx*/, char a2)
{
    uint8_t flagWasSet = CSF_AT(uint8_t, self, 0x8);

    self->vtbl = CSF_VTBL(0x008619E0);

    if (flagWasSet)
        CSF_AT(uint8_t, self, 0x8) = 0;

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x006E0420, C_008619e0_Destroy, "auto");
