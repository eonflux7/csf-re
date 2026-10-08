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

// Compares two 44-byte settings blocks field by field (dwords, one float at +0x0C,
// two bytes at +0x10/+0x11, then dwords through +0x2C). Returns 0 when identical,
// 1 as soon as any field differs. A NaN in the float field counts as different.
static int __fastcall MenuVideo_IsSettingsDifferent(void* self, void* /*edx*/, void* other)
{
    if (CSF_AT(uint32_t, self, 0x00) != CSF_AT(uint32_t, other, 0x00)) return 1;
    if (CSF_AT(uint32_t, self, 0x04) != CSF_AT(uint32_t, other, 0x04)) return 1;
    if (CSF_AT(uint32_t, self, 0x08) != CSF_AT(uint32_t, other, 0x08)) return 1;
    if (CSF_AT(float, self, 0x0C) != CSF_AT(float, other, 0x0C)) return 1;
    if (CSF_AT(uint8_t, self, 0x10) != CSF_AT(uint8_t, other, 0x10)) return 1;
    if (CSF_AT(uint8_t, self, 0x11) != CSF_AT(uint8_t, other, 0x11)) return 1;
    if (CSF_AT(uint32_t, self, 0x14) != CSF_AT(uint32_t, other, 0x14)) return 1;
    if (CSF_AT(uint32_t, self, 0x18) != CSF_AT(uint32_t, other, 0x18)) return 1;
    if (CSF_AT(uint32_t, self, 0x1C) != CSF_AT(uint32_t, other, 0x1C)) return 1;
    if (CSF_AT(uint32_t, self, 0x20) != CSF_AT(uint32_t, other, 0x20)) return 1;
    if (CSF_AT(uint32_t, self, 0x24) != CSF_AT(uint32_t, other, 0x24)) return 1;
    if (CSF_AT(uint32_t, self, 0x28) != CSF_AT(uint32_t, other, 0x28)) return 1;
    if (CSF_AT(uint32_t, self, 0x2C) != CSF_AT(uint32_t, other, 0x2C)) return 1;
    return 0;
}

HOOK(0x549C10, MenuVideo_IsSettingsDifferent, "auto");
