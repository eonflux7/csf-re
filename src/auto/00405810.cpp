#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0082d510_ResetState(C_0082d510* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x11) = 0;
    return 0;
}

HOOK(0x00405810, C_0082d510_ResetState, "auto");
