#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns true when the byte flag at offset 0x5F of the object equals 1.
static int __fastcall IsStateByteAt5FEqual1(void* self, void* /*edx*/)
{
    return CSF_AT(uint8_t, self, 0x5f) == 1;
}

HOOK(0x007483A0, IsStateByteAt5FEqual1, "auto");
