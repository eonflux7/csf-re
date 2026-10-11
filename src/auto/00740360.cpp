#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies the byte at +0x6C into +0xBC when the flag byte at +0x6F does not have bit 1 set
// and the two bytes differ. Returns 1 when the copy happened, otherwise 0.
static char __fastcall NetCogibleDDL_SyncByteFieldIfChanged(void* self, void* /*edx*/)
{
    char result = 0;
    if ((CSF_AT(uint8_t, self, 0x6F) & 2) == 0)
    {
        uint8_t sourceValue = CSF_AT(uint8_t, self, 0x6C);
        if (CSF_AT(uint8_t, self, 0xBC) != sourceValue)
        {
            CSF_AT(uint8_t, self, 0xBC) = sourceValue;
            return 1;
        }
    }
    return result;
}

HOOK(0x740360, NetCogibleDDL_SyncByteFieldIfChanged, "auto");
