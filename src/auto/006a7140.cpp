#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Returns the 32-bit value stored at offset 0x2B0 of the object.
static int __fastcall C_006a7140_GetField2B0(void* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, self, 0x2B0));
}

HOOK(0x6A7140, C_006a7140_GetField2B0, "auto");
