#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(double, __cdecl, Camera_ConvertSetupValue, 0x006D29B0, (float a1));
CSF_FUNC(int, __fastcall, Camera_ApplySetup, 0x006D3250, (void* self, void* edx));

static int __fastcall C_008655e8_Setup(
    C_008655e8* self, void* /*edx*/, int setupAddress, uint32_t context)
{
    void* setup = reinterpret_cast<void*>(setupAddress);

    if (CSF_AT(uint32_t, setup, 0x00) ||
        CSF_AT(uint32_t, setup, 0x04) ||
        CSF_AT(uint32_t, setup, 0x08) ||
        CSF_AT(uint32_t, setup, 0x0C))
    {
        self->field_0034 = CSF_AT(uint32_t, setup, 0x00);
        self->field_0038 = CSF_AT(uint32_t, setup, 0x04);
        self->field_003c = CSF_AT(uint32_t, setup, 0x08);
        self->field_0040 = CSF_AT(uint32_t, setup, 0x0C);
    }
    else
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* object = LookupObjectById(global, nullptr);

        self->field_0034 = 0;
        self->field_0038 = 0;

        int height =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x24 / 4])(object, nullptr);
        int width =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x20 / 4])(object, nullptr);

        uint32_t right = self->field_0034 + static_cast<uint32_t>(width);
        uint32_t bottom = self->field_0038 + static_cast<uint32_t>(height);
        self->field_003c = right;
        self->field_0040 = bottom;
    }

    CSF_AT(uint32_t, self, 0x44) = CSF_AT(uint32_t, setup, 0x10);
    self->field_0048 = CSF_AT(uint32_t, setup, 0x14);
    self->field_004c = CSF_AT(uint32_t, setup, 0x18);

    float convertedValue =
        static_cast<float>(Camera_ConvertSetupValue(CSF_AT(float, setup, 0x1C)));
    std::memcpy(&self->field_0050, &convertedValue, sizeof(convertedValue));

    self->field_0054 = context;
    self->field_0058 = CSF_AT(uint32_t, setup, 0x20);
    return Camera_ApplySetup(self, nullptr);
}

HOOK(0x006D3450, C_008655e8_Setup, "auto");
