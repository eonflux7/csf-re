#include "globals.h"
#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x0042DEF0 (8 bytes): clears the byte flag at offset 0x23D of the object.
static void __fastcall C_42def0_ClearByteFlag(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x23d) = 0;
}

HOOK(0x42DEF0, C_42def0_ClearByteFlag, "auto");
