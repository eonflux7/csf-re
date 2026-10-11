#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, VehicleWheel_GrowArray, 0x0040CDB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, VehicleWheel_BaseInit, 0x00418A50, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, VehicleWheel_SetupA, 0x00419A00, (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, VehicleWheel_SetupB, 0x00419A80, (void* self, void* edx, int a2, int a3, float a4));

static void* __fastcall C_0082e280_Construct(C_0082e280* self, void* /*edx*/, char a2)
{
    VehicleWheel_BaseInit(self, nullptr, a2);
    self->vtbl = CSF_VTBL(0x0082E280);

    VehicleWheel_SetupA(self, nullptr, 0, 0, 20.0f);
    VehicleWheel_SetupB(self, nullptr, 3, 0, 20.0f);
    VehicleWheel_SetupA(self, nullptr, 1, 0, 20.0f);
    VehicleWheel_SetupB(self, nullptr, 4, 0, 20.0f);

    void* arr = &self->field_0010;
    if (self->field_0014 < 2) {
        VehicleWheel_GrowArray(arr, nullptr, 2 - (int)self->field_0014);
    }

    int flagsA = 0x100;
    int flagsB = 0;

    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4)) {
        VehicleWheel_GrowArray(arr, nullptr, (int)CSF_AT(uint32_t, arr, 0xC));
    }
    {
        uint32_t idx = CSF_AT(uint32_t, arr, 8);
        uint32_t base = CSF_AT(uint32_t, arr, 0);
        CSF_AT(uint32_t, arr, 8) = idx + 1;
        uint32_t* e = reinterpret_cast<uint32_t*>(base + idx * 24);
        e[0] = 0;
        e[1] = 0x456;
        e[2] = 0;
        e[3] = (uint32_t)flagsA;
        e[4] = 7;
        e[5] = 0xFFFFFFFFu;
    }

    if (CSF_AT(uint32_t, arr, 8) >= CSF_AT(uint32_t, arr, 4)) {
        VehicleWheel_GrowArray(arr, nullptr, (int)CSF_AT(uint32_t, arr, 0xC));
    }
    {
        uint32_t idx = CSF_AT(uint32_t, arr, 8);
        uint32_t base = CSF_AT(uint32_t, arr, 0);
        CSF_AT(uint32_t, arr, 8) = idx + 1;
        uint32_t* e = reinterpret_cast<uint32_t*>(base + idx * 24);
        e[0] = 1;
        e[1] = 0x44C;
        e[2] = 0x44D;
        e[3] = (uint32_t)flagsB;
        e[4] = 0xFFFFFFFFu;
        e[5] = 0xFFFFFFFFu;
    }

    self->field_008c = 0.0f;
    return self;
}

HOOK(0x0041B9B0, C_0082e280_Construct, "auto");
