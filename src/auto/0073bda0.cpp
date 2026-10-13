// Resets the spawn-point object's cached state fields to their "none" values
// (-1 for the int fields, 0 for the flag bytes) and returns -1.

#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall MpSpawnPoints_ResetSpawnObject(void* self, void* /*edx*/)
{
    int result = -1;

    if (CSF_AT(int, self, 0x5C) != -1)
        CSF_AT(int, self, 0x5C) = -1;

    if (CSF_AT(uint8_t, self, 0x44))
        CSF_AT(uint8_t, self, 0x44) = 0;

    if (CSF_AT(int, self, 0x68) != -1 || CSF_AT(int, self, 0x6C) != -1)
    {
        CSF_AT(int, self, 0x68) = -1;
        CSF_AT(int, self, 0x6C) = -1;
    }

    if (CSF_AT(uint8_t, self, 0x47))
        CSF_AT(uint8_t, self, 0x47) = 0;

    return result;
}

HOOK(0x73BDA0, MpSpawnPoints_ResetSpawnObject, "auto");
