#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the byte stored at offset 0xCB.  The field lies inside the pad_00a8
// array of the base class, so it is accessed by raw offset.
static char __fastcall C_00864288_GetByte_00cb(C_00864288* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0xcb));
}

HOOK(0x007180C0, C_00864288_GetByte_00cb, "auto");
