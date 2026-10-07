#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets bit 0 of the flags word at offset 0x8 and increments the counter at 0xC.
static void __fastcall C_004de770_LockAndAddRef(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x8) |= 1u;
    ++CSF_AT(uint32_t, self, 0xC);
}

HOOK(0x4DE770, C_004de770_LockAndAddRef, "auto");
