#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies a 3-dword record from *source into the trailing fields at +0x0C..+0x14
// and returns the third dword.
static int __fastcall C_00861d48_CopyTrailingRecord(C_00861d48* self, void* /*edx*/, const uint32_t* source)
{
    CSF_AT(uint32_t, self, 0x0C) = source[0];
    CSF_AT(uint32_t, self, 0x10) = source[1];
    CSF_AT(uint32_t, self, 0x14) = source[2];
    return static_cast<int>(source[2]);
}

HOOK(0x6E6B40, C_00861d48_CopyTrailingRecord, "auto");
