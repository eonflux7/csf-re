#include <cstdint>
#include "globals.h"
#include "hook.h"

static uint32_t* __fastcall C_006f2820_SetStoredValue(
    uint32_t* self, void* /*edx*/, uint32_t* value)
{
    CSF_AT(uint32_t, self, 0x20) = *value;
    return value;
}

HOOK(0x006F2820, C_006f2820_SetStoredValue, "auto");
