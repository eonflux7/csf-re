#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, VehicleWheel_GrowArray, 0x0040CDB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082e1e8_Construct, 0x00418A50, (C_0082e1e8* self, void* , char a2));

static int* __fastcall C_0082db60_Initialize(C_0082db60* self, void* /*edx*/)
{
    C_0082e1e8_Construct(self, nullptr, 0);
    self->vtbl = CSF_VTBL(0x0082DB60);

    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_0090 = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;

    void* array = reinterpret_cast<uint8_t*>(self) + 0x10;
    if (!self->field_0014)
        VehicleWheel_GrowArray(array, nullptr, 1);

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
        VehicleWheel_GrowArray(array, nullptr, CSF_AT(int, self, 0x1C));

    const uint32_t index = CSF_AT(uint32_t, self, 0x18);
    auto* entry = reinterpret_cast<uint8_t*>(self->field_0010 + 24 * index);
    CSF_AT(uint32_t, self, 0x18) = index + 1;

    CSF_AT(int, entry, 0x00) = 0;
    CSF_AT(int, entry, 0x04) = 1100;
    CSF_AT(int, entry, 0x08) = 1100;
    CSF_AT(int, entry, 0x0C) = 0;
    CSF_AT(int, entry, 0x10) = -1;
    CSF_AT(int, entry, 0x14) = -1;

    return reinterpret_cast<int*>(self);
}

HOOK(0x0040CEB0, C_0082db60_Initialize, "auto");
