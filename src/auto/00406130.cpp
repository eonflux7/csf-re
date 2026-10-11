#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_4086B0, 0x004086B0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_6D9EB0, 0x006D9EB0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0082d648_Construct(C_0082d648* self, void* /*edx*/)
{
    sub_4086B0(self, nullptr);

    self->vtbl = CSF_VTBL(0x0082D648);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D628)));

    // Growable array of 12-byte entries embedded at +0x2C:
    // +0x2C data pointer, +0x30 capacity (field_0030), +0x34 count, +0x38 grow argument.
    uint32_t capacity = self->field_0030;
    if (capacity < 3) {
        sub_6D9EB0(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr,
                   static_cast<int>(3 - capacity));
    }

    uint32_t* src = reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x0089905C));
    int remaining = 3;
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

HOOK(0x00406130, C_0082d648_Construct, "auto");
