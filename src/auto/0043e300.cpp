#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the two heap buffers owned by this campaign record (when they grew past
// the 15-element inline capacity), then resets both to empty with inline capacity 15.
// Returns the result of the last free performed, or the last value read if none.
static unsigned int __fastcall Campaign_ReleaseAndResetRecordBuffers(void* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint32_t, self, 0x34);

    if (result >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x20);
        if (result)
            result = static_cast<uint32_t>(Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
    }
    CSF_AT(uint32_t, self, 0x34) = 15;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint8_t, self, 0x20) = 0;

    if (CSF_AT(uint32_t, self, 0x18) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x04);
        if (result)
            result = static_cast<uint32_t>(Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
    }
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint8_t, self, 0x04) = 0;

    return result;
}

HOOK(0x43E300, Campaign_ReleaseAndResetRecordBuffers, "auto");
