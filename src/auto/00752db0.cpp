#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Class unknown (only an 8-byte setter); clears the byte flag at offset 0x19E.
static void __fastcall C_00752db0_ClearFlag(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x19e) = 0;
}

HOOK(0x752DB0, C_00752db0_ClearFlag, "auto");
