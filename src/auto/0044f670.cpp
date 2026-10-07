#include <cstdint>
#include "globals.h"
#include "hook.h"

static void __fastcall C_0044f670_ClearByte128(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x128) = 0;
}

HOOK(0x44F670, C_0044f670_ClearByte128, "auto");
