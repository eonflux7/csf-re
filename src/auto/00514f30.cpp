#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees the two heap buffers owned by a save-slot record and resets its
// bookkeeping fields. The 0x38 buffer is always freed when non-null. The
// buffer at +4 is only freed when the capacity at +0x18 is 8 or more
// (small-buffer optimization, unsigned compare).
static int __stdcall ResetSaveSlotBuffers(int* self)
{
    int result = CSF_AT(int, self, 0x38);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, self, 0x38)));

    CSF_AT(int, self, 0x38) = 0;
    CSF_AT(int, self, 0x3C) = 0;
    CSF_AT(int, self, 0x40) = 0;

    if (CSF_AT(uint32_t, self, 0x18) >= 8)
    {
        result = CSF_AT(int, self, 0x04);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, self, 0x04)));
    }

    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 7;
    CSF_AT(uint16_t, self, 0x04) = 0;
    return result;
}

HOOK(0x514F30, ResetSaveSlotBuffers, "auto");
