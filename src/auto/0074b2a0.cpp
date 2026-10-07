#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x0074B2A0: sets the byte flag at offset +4 to 1 and returns 1.
static char __fastcall C_0074b2a0_SetFlag(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x4) = 1;
    return 1;
}

HOOK(0x74B2A0, C_0074b2a0_SetFlag, "auto");
