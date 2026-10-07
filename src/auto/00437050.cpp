#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads the byte flag at offset 0xD4 of this object (offset lies inside the
// base-class pad_00a8 array, so it is accessed via CSF_AT).
static char __fastcall C_00864288_GetByteD4(C_00864288* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0xd4));
}

HOOK(0x00437050, C_00864288_GetByteD4, "auto");
