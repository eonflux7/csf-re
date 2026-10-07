#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x006E42F0 (9 bytes): if the one-byte flag at offset 0 of the object is set,
// clear it to false. The original reads the byte once, and only writes 0 when
// it was non-zero.
static void __fastcall ClearFlagIfSet(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0))
        CSF_AT(uint8_t, self, 0) = 0;
}

HOOK(0x6E42F0, ClearFlagIfSet, "auto");
