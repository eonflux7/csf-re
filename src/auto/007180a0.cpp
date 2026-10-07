#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x007180A0: returns the byte field at offset 0xC9 of the object.
static char __fastcall C_00864288_GetField00C9(C_00864288* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0xC9));
}

HOOK(0x007180A0, C_00864288_GetField00C9, "auto");
