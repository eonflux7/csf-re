#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, GameBddLoader_ConfigureDisplayState, 0x0050C450, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static int __fastcall GameBddLoader_InitializeDisplayResources(void* self, void* /*edx*/)
{
    CSF_AT(void*, self, 0xAC) = nullptr;

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(void*, self, 0xAC) =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controls, 0)[0x44 / 4])(controls, nullptr);

    alignas(8) uint8_t colorBytes[0x10];
    CSF_AT(uint32_t, colorBytes, 0x00) = 0;
    CSF_AT(uint32_t, colorBytes, 0x04) = 0;
    CSF_AT(uint32_t, colorBytes, 0x08) = 0;
    CSF_AT(float, colorBytes, 0x0C) = 1.0f;

    CSF_AT(uint32_t, colorBytes, 0x00) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB34);
    CSF_AT(uint32_t, colorBytes, 0x04) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB38);
    CSF_AT(uint32_t, colorBytes, 0x08) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB3C);
    CSF_AT(uint32_t, colorBytes, 0x0C) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB40);

    void* display = CSF_AT(void*, self, 0xAC);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, display, 0)[0x2C / 4])(display, nullptr, colorBytes);

    controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(void*, self, 0xB0) =
        reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, controls, 0)[0x54 / 4])(
                controls, nullptr, CSF_AT(uint32_t, self, 0x8C));

    void* viewport = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, viewport, 0)[0x18 / 4])(viewport, nullptr);
    unsigned int width = CSF_AT(unsigned int, dimensions, 0);

    viewport = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, viewport, 0)[0x18 / 4])(viewport, nullptr);
    unsigned int height = CSF_AT(unsigned int, dimensions, 0x04);

    alignas(8) uint8_t scaleBytes[0x0C];
    CSF_AT(float, scaleBytes, 0x00) =
        static_cast<float>(width) / static_cast<float>(g_ScreenWidth);
    CSF_AT(float, scaleBytes, 0x04) =
        static_cast<float>(height) / static_cast<float>(g_ScreenHeight);

    void* resource = CSF_AT(void*, self, 0xB0);
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, resource, 0)[0x98 / 4])(
            resource, nullptr, reinterpret_cast<float*>(scaleBytes));

    resource = CSF_AT(void*, self, 0xB0);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, resource, 0)[0xA8 / 4])(resource, nullptr, 0);

    return static_cast<int>(GameBddLoader_ConfigureDisplayState(
        static_cast<uint8_t*>(self) + 0xB4, nullptr, 10));
}

HOOK(0x0050F980, GameBddLoader_InitializeDisplayResources, "auto");
