#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, VehicleWheel_GrowArray, 0x0040CDB0,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082dc88_Initialize, 0x0040D300,
         (void* self, void* edx, char a2));

static uint32_t* __fastcall C_0082dcf0_Initialize(
    C_0082dcf0* self, void* /*edx*/, char a2)
{
    C_0082dc88_Initialize(self, nullptr, a2);
    self->vtbl = CSF_VTBL(0x0082DCF0);

    void* records = reinterpret_cast<uint8_t*>(self) + 0x10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;

    uint32_t capacity = self->field_0014;
    if (capacity < 3)
        VehicleWheel_GrowArray(records, nullptr, static_cast<int>(3 - capacity));

    // The original initializes only the low half of this stack value.
    // Its uninitialized high half is represented as zero.
    const uint32_t recordFlags = 0;

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
        VehicleWheel_GrowArray(
            records, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x1c)));

    uint32_t index = CSF_AT(uint32_t, self, 0x18);
    uint32_t storage = self->field_0010;
    CSF_AT(uint32_t, self, 0x18) = index + 1;
    auto* record = reinterpret_cast<uint32_t*>(storage + 24 * index);
    CSF_AT(uint32_t, record, 0x00) = 1;
    CSF_AT(uint32_t, record, 0x04) = 1500;
    CSF_AT(uint32_t, record, 0x08) = 1600;
    CSF_AT(uint32_t, record, 0x0c) = recordFlags;
    CSF_AT(uint32_t, record, 0x10) = -1;
    CSF_AT(uint32_t, record, 0x14) = -1;

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
        VehicleWheel_GrowArray(
            records, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x1c)));

    index = CSF_AT(uint32_t, self, 0x18);
    storage = self->field_0010;
    CSF_AT(uint32_t, self, 0x18) = index + 1;
    record = reinterpret_cast<uint32_t*>(storage + 24 * index);
    CSF_AT(uint32_t, record, 0x00) = 1;
    CSF_AT(uint32_t, record, 0x04) = 1100;
    CSF_AT(uint32_t, record, 0x08) = 1200;
    CSF_AT(uint32_t, record, 0x0c) = recordFlags;
    CSF_AT(uint32_t, record, 0x10) = -1;
    CSF_AT(uint32_t, record, 0x14) = -1;

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
        VehicleWheel_GrowArray(
            records, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x1c)));

    index = CSF_AT(uint32_t, self, 0x18);
    storage = self->field_0010;
    CSF_AT(uint32_t, self, 0x18) = index + 1;
    record = reinterpret_cast<uint32_t*>(storage + 24 * index);
    CSF_AT(uint32_t, record, 0x00) = 1;
    CSF_AT(uint32_t, record, 0x04) = 1300;
    CSF_AT(uint32_t, record, 0x08) = 1400;
    CSF_AT(uint32_t, record, 0x0c) = recordFlags;
    CSF_AT(uint32_t, record, 0x10) = -1;
    CSF_AT(uint32_t, record, 0x14) = -1;

    self->field_0050 = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = -1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0040DE10, C_0082dcf0_Initialize, "auto");
