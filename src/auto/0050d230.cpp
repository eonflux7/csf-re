#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets a block of menu-state fields on the object in ecx.
// Original: int __thiscall sub_50D230(int this); returns 0, pops nothing.
static int __fastcall MenuSystem_ResetStateFields(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x89) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint8_t, self, 0x94) = 0;
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint8_t, self, 0xA8) = 0;
    CSF_AT(uint8_t, self, 0xA9) = 0;
    return 0;
}

HOOK(0x0050D230, MenuSystem_ResetStateFields, "auto");
