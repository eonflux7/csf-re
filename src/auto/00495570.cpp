#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Reads and returns the byte at offset 0x3DE of the object (ecx = this).
static char __fastcall C_00495570_GetByte3DE(void* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0x3DE));
}

HOOK(0x495570, C_00495570_GetByte3DE, "auto");
