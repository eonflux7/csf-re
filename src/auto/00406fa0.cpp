#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, sub_551CF0, 0x00551CF0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0082d77c_InitializeDefaultEntries(
    C_0082d77c* self, void* /*edx*/)
{
    self->field_0004 = static_cast<uint32_t>(
        reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082D428)));
    self->vtbl = CSF_VTBL(0x0082D77C);
    self->field_0004 = static_cast<uint32_t>(
        reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082D760)));

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_001c = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 10;
    self->field_0008 = 0;

    void* entries = reinterpret_cast<uint8_t*>(self) + 0x1C;
    const uint32_t capacity = CSF_AT(uint32_t, self, 0x20);
    if (capacity < 3)
        sub_551CF0(entries, nullptr, static_cast<int>(3 - capacity));

    const uint32_t* defaultEntry =
        reinterpret_cast<const uint32_t*>(CSF_ADDR_DATA(0x00899080));

    int remaining = 3;
    do
    {
        if (CSF_AT(uint32_t, self, 0x24) >=
            CSF_AT(uint32_t, self, 0x20))
        {
            sub_551CF0(entries, nullptr,
                static_cast<int>(CSF_AT(uint32_t, self, 0x28)));
        }

        const uint32_t count = CSF_AT(uint32_t, self, 0x24);
        uint32_t* destination =
            reinterpret_cast<uint32_t*>(self->field_001c) + 2 * count;
        CSF_AT(uint32_t, self, 0x24) = count + 1;

        destination[0] = defaultEntry[0];
        destination[1] = defaultEntry[1];
        defaultEntry += 2;
        --remaining;
    } while (remaining);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00406FA0, C_0082d77c_InitializeDefaultEntries, "auto");
