#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4B2210, 0x004B2210,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

static int __fastcall S_004ad990_ApplyCombatDestination(
    S_004ad990* self, void* /*edx*/, int destinationAddress)
{
    auto* destination = reinterpret_cast<uint8_t*>(destinationAddress);

    if (self->field_0038 != CSF_AT(uint32_t, destination, 0x0C) ||
        self->field_0040 != CSF_AT(float, destination, 0x14) ||
        self->field_0044 != CSF_AT(float, destination, 0x18) ||
        self->field_0048 != CSF_AT(float, destination, 0x1C))
    {
        self->field_00a4 = 0;
    }

    sub_4B2210(
        reinterpret_cast<uint8_t*>(self) + 0x28, nullptr,
        CSF_AT(int, destination, 0x04),
        reinterpret_cast<int>(destination + 0x08));

    self->field_0040 = CSF_AT(float, destination, 0x14);
    self->field_0044 = CSF_AT(float, destination, 0x18);
    self->field_0048 = CSF_AT(float, destination, 0x1C);
    self->field_0050 = -1;
    self->field_0054 = -1;

    uint32_t* objectPosition = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0000), nullptr);

    self->field_005c = 0;
    CSF_AT(float, self, 0x70) = self->field_0040;
    CSF_AT(float, self, 0x74) = self->field_0044;
    CSF_AT(float, self, 0x78) = self->field_0048;
    self->field_0060 = 1065353216;
    CSF_AT(uint32_t, self, 0x64) = objectPosition[0];
    CSF_AT(uint32_t, self, 0x68) = objectPosition[1];
    int result = static_cast<int>(objectPosition[2]);
    CSF_AT(int, self, 0x6C) = result;

    self->field_0098 = CSF_AT(float, destination, 0x20);
    self->field_009c = 0.0f;
    self->field_00a8 = 1;
    self->field_00a9 = 0;
    return result;
}

HOOK(0x004B8EF0, S_004ad990_ApplyCombatDestination, "auto");
