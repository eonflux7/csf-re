#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_008655e8_SetCameraViewport, 0x006D29F0, (C_008655e8* self, void* , int left, int top, int width, int height));

static int __fastcall GfxLightObject_ApplyViewportSettings(
    void* self, void* /*edx*/, const void* settings)
{
    CSF_AT(uint8_t, self, 0x0C) = CSF_AT(uint8_t, settings, 0x25);
    CSF_AT(uint32_t, self, 0x40) = CSF_AT(uint32_t, settings, 0x00);
    CSF_AT(uint32_t, self, 0x24) = CSF_AT(uint32_t, settings, 0x14);
    CSF_AT(uint32_t, self, 0x28) = CSF_AT(uint32_t, settings, 0x18);
    CSF_AT(uint32_t, self, 0x2C) = CSF_AT(uint32_t, settings, 0x1C);
    CSF_AT(uint32_t, self, 0x30) = CSF_AT(uint32_t, settings, 0x20);
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x54) = CSF_AT(uint8_t, settings, 0x24);

    int result;
    if (CSF_AT(uint32_t, settings, 0x04) ||
        CSF_AT(uint32_t, settings, 0x08) ||
        CSF_AT(uint32_t, settings, 0x0C) ||
        CSF_AT(uint32_t, settings, 0x10))
    {
        CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, settings, 0x04);
        CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, settings, 0x08);
        CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, settings, 0x0C);
        result = CSF_AT(int, settings, 0x10);
        CSF_AT(int, self, 0x20) = result;
    }
    else
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* display = LookupObjectById(global, nullptr);
        CSF_AT(uint32_t, self, 0x14) = 0;
        CSF_AT(uint32_t, self, 0x18) = 0;

        const int height =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, display, 0)[0x24 / 4])(display, nullptr);
        result =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr);

        const uint32_t right =
            CSF_AT(uint32_t, self, 0x14) + static_cast<uint32_t>(result);
        const uint32_t bottom =
            CSF_AT(uint32_t, self, 0x18) + static_cast<uint32_t>(height);
        CSF_AT(uint32_t, self, 0x1C) = right;
        CSF_AT(uint32_t, self, 0x20) = bottom;
    }

    auto* camera = CSF_AT(C_008655e8*, self, 0x10);
    if (camera)
    {
        const int height = static_cast<int>(
            CSF_AT(uint32_t, self, 0x20) - CSF_AT(uint32_t, self, 0x18));
        const int width = static_cast<int>(
            CSF_AT(uint32_t, self, 0x1C) - CSF_AT(uint32_t, self, 0x14));
        const int top = CSF_AT(int, self, 0x18);
        const int left = CSF_AT(int, self, 0x14);
        return C_008655e8_SetCameraViewport(
            camera, nullptr, left, top, width, height);
    }
    return result;
}

HOOK(0x006FC310, GfxLightObject_ApplyViewportSettings, "auto");
