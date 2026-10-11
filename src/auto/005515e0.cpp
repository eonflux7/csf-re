#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x005515E0 (16 bytes): stores a2 at this+0x13CC (dword index 1267) and returns it.
static int __fastcall MpSpawnPoints_SetSpawnValue(void* self, void* /*edx*/, int value)
{
    CSF_AT(uint32_t, self, 0x13CC) = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x5515E0, MpSpawnPoints_SetSpawnValue, "auto");
