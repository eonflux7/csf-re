#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Caches a 2-component float pair (a2[0], a2[1]) in the object. When either
// component differs from the stored value (or is NaN), sets the change flag
// at +0x20, stores the new floats at +0x2C/+0x30, and stores the truncated
// integer parts of both components at +0x24/+0x28 as 32-bit values.
static void __fastcall C_00867260_CacheVec2IfChanged(C_00867260* self, void* /*edx*/, float* a2)
{
    float& storedX = CSF_AT(float, self, 0x2C);
    float& storedY = CSF_AT(float, self, 0x30);

    if (storedX != a2[0] || storedY != a2[1])
    {
        CSF_AT(uint8_t, self, 0x20) = 1;

        storedX = a2[0];
        storedY = a2[1];

        CSF_AT(uint32_t, self, 0x24) = (uint32_t)(int64_t)a2[0];
        CSF_AT(uint32_t, self, 0x28) = (uint32_t)(int64_t)a2[1];
    }
}

HOOK(0x007374B0, C_00867260_CacheVec2IfChanged, "auto");
