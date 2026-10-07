#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00842e08_ResetState(C_00842e08* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x80) = 0;
    CSF_AT(uint8_t, self, 0xA4) = 0;
    CSF_AT(uint8_t, self, 0xA5) = 0;
    CSF_AT(uint8_t, self, 0xA6) = 0;
    CSF_AT(uint8_t, self, 0xA7) = 0;
    CSF_AT(uint8_t, self, 0xA8) = 0;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    return 0;
}

HOOK(0x0053D2D0, C_00842e08_ResetState, "auto");
