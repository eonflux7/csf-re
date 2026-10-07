#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns the byte flag stored at offset 0x331 and clears it (read-and-reset).
static char __fastcall ConsumeByteFlag(void* self, void* /*edx*/)
{
    uint8_t& flag = CSF_AT(uint8_t, self, 0x331);
    char result = static_cast<char>(flag);
    flag = 0;
    return result;
}

HOOK(0x748460, ConsumeByteFlag, "auto");
