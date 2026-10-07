#include <cstdint>
#include "hook.h"
#include "core/memory.h"

// Returns the single byte stored at offset 0x184 of the object.
static uint8_t __fastcall C_537220_GetByte184(void* self, void* /*edx*/)
{
    return CSF_AT(uint8_t, self, 0x184);
}

HOOK(0x537220, C_537220_GetByte184, "auto");
