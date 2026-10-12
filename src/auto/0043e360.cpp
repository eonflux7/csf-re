#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static uint32_t __fastcall Campaign_ClearStrings(uint32_t* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint32_t, self, 0xB4);
    if (result >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0xA0);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0xB4) = 15;
    CSF_AT(uint32_t, self, 0xB0) = 0;
    CSF_AT(uint8_t, self, 0xA0) = 0;

    if (CSF_AT(uint32_t, self, 0x98) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x84);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0x98) = 15;
    CSF_AT(uint32_t, self, 0x94) = 0;
    CSF_AT(uint8_t, self, 0x84) = 0;

    if (CSF_AT(uint32_t, self, 0x78) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x64);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0x78) = 15;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint8_t, self, 0x64) = 0;

    if (CSF_AT(uint32_t, self, 0x58) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x44);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0x58) = 15;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint8_t, self, 0x44) = 0;

    if (CSF_AT(uint32_t, self, 0x38) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x24);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0x38) = 15;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x24) = 0;

    if (CSF_AT(uint32_t, self, 0x1C) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x08);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint8_t, self, 0x08) = 0;

    return result;
}

HOOK(0x0043E360, Campaign_ClearStrings, "auto");
