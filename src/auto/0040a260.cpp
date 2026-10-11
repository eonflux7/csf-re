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

CSF_FUNC(uint32_t*, __fastcall, sub_4086B0, 0x004086B0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_6D9EB0, 0x006D9EB0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0082da40_Construct(C_0082da40* self, void* /*edx*/)
{
    sub_4086B0(self, nullptr);

    self->vtbl = CSF_VTBL(0x0082DA40);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082DA20)));

    // Growable array of 12-byte entries embedded at +0x2C:
    // +0x2C data pointer, +0x30 capacity (field_0030), +0x34 count, +0x38 grow argument.
    uint32_t capacity = self->field_0030;
    if (capacity < 0x1A) {
        sub_6D9EB0(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr,
                   static_cast<int>(0x1A - capacity));
    }

    uint32_t* src = reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x00899240));
    int remaining = 0x1A;
    do {
        if (CSF_AT(uint32_t, self, 0x34) >= self->field_0030) {
            sub_6D9EB0(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr,
                       static_cast<int>(CSF_AT(uint32_t, self, 0x38)));
        }

        uint32_t count = CSF_AT(uint32_t, self, 0x34);
        uint32_t base = CSF_AT(uint32_t, self, 0x2C);
        CSF_AT(uint32_t, self, 0x34) = count + 1;

        uint32_t* dst = reinterpret_cast<uint32_t*>(base + 12u * count);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        src += 3;
        --remaining;
    } while (remaining != 0);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0040A260, C_0082da40_Construct, "auto");
