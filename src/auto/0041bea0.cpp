#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, VehicleWheel_GrowArray, 0x0040CDB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, VehicleBase_Construct, 0x00418A50, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, VehicleWheel_SetupLeft, 0x00419A00, (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, VehicleWheel_SetupRight, 0x00419A80, (void* self, void* edx, int a2, int a3, float a4));

static void* __fastcall C_0082e2e8_Construct(C_0082e2e8* self, void* /*edx*/, char a2)
{
    VehicleBase_Construct(self, nullptr, a2);
    self->vtbl = CSF_VTBL(0x0082E2E8);

    VehicleWheel_SetupLeft(self, nullptr, 0, 0, 12.0f);
    VehicleWheel_SetupRight(self, nullptr, 2, 0, 12.0f);
    VehicleWheel_SetupLeft(self, nullptr, 1, 0, 12.0f);
    VehicleWheel_SetupRight(self, nullptr, 3, 0, 12.0f);

    void* arr = reinterpret_cast<uint8_t*>(self) + 0x10;
    uint32_t capacity = self->field_0014;
    if (capacity < 2)
        VehicleWheel_GrowArray(arr, nullptr, 2 - (int)capacity);

    int flagsA = 256;
    int flagsB = 256;
    int tail = -1;

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
        VehicleWheel_GrowArray(arr, nullptr, (int)CSF_AT(uint32_t, self, 0x1C));
    {
        uint32_t idx = CSF_AT(uint32_t, self, 0x18);
        CSF_AT(uint32_t, self, 0x18) = idx + 1;
        uint8_t* e = reinterpret_cast<uint8_t*>(self->field_0010) + 24 * idx;
        CSF_AT(uint32_t, e, 0x00) = 0;
        CSF_AT(uint32_t, e, 0x04) = 0x456;
        CSF_AT(uint32_t, e, 0x08) = 0;
        CSF_AT(int, e, 0x0C) = flagsA;
        CSF_AT(uint32_t, e, 0x10) = 5;
        CSF_AT(int, e, 0x14) = -1;
    }

    if (CSF_AT(uint32_t, self, 0x18) >= self->field_0014)
        VehicleWheel_GrowArray(arr, nullptr, (int)CSF_AT(uint32_t, self, 0x1C));
    {
        uint32_t idx = CSF_AT(uint32_t, self, 0x18);
        CSF_AT(uint32_t, self, 0x18) = idx + 1;
        uint8_t* e = reinterpret_cast<uint8_t*>(self->field_0010) + 24 * idx;
        CSF_AT(uint32_t, e, 0x00) = 0;
        CSF_AT(uint32_t, e, 0x04) = 0x460;
        CSF_AT(uint32_t, e, 0x08) = 0;
        CSF_AT(int, e, 0x0C) = flagsB;
        CSF_AT(uint32_t, e, 0x10) = 6;
        CSF_AT(int, e, 0x14) = tail;
    }

    self->field_008c = 0.0f;
    return self;
}

HOOK(0x0041BEA0, C_0082e2e8_Construct, "auto");
