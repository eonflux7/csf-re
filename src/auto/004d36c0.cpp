#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns whether the status flags at +0x44 pass the test for the given mode code.
// Modes 1/3 and 2/4 succeed when their bits are clear; mode 5 succeeds when either bit is set.
static bool __fastcall TestStatusBitsForMode(void* self, void* /*edx*/, int mode)
{
    bool result = false;

    switch (mode)
    {
    case 1:
    case 3:
        result = (CSF_AT(uint32_t, self, 0x44) & 0x14) == 0;
        break;
    case 2:
    case 4:
        result = (CSF_AT(uint32_t, self, 0x44) & 0x10) == 0;
        break;
    case 5:
        result = (CSF_AT(uint8_t, self, 0x44) & 0x14) != 0;
        break;
    default:
        break;
    }

    return result;
}

HOOK(0x4D36C0, TestStatusBitsForMode, "auto");
