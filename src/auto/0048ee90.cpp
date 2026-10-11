#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t __fastcall C_00833908_SetFlagBit800(
    C_00833908* self, void* /*edx*/, char enable)
{
    uint32_t flags = CSF_AT(uint32_t, self, 0x170);
    if (enable) {
        flags = flags | 0x800u;
    } else {
        flags = flags & 0xFFFFF7FFu;
    }
    CSF_AT(uint32_t, self, 0x170) = flags;
    return flags;
}

HOOK(0x48EE90, C_00833908_SetFlagBit800, "auto");
