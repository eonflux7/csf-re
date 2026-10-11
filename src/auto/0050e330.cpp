#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static int __fastcall GameBddLoader_CreateScreenOverlay(void* self, void* /*edx*/)
{
    if (!CSF_AT(uint32_t, self, 0x8C))
        return 0;

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    CSF_AT(void*, self, 0x104) =
        reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, controls, 0)[0x54 / 4])(
                controls, nullptr, CSF_AT(uint32_t, self, 0x8C));

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t width = CSF_AT(uint32_t, dimensions, 0);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t height = CSF_AT(uint32_t, dimensions, 0x4);

    alignas(8) uint8_t sizeBytes[0xC];
    CSF_AT(uint32_t, sizeBytes, 0x8) = height;
    CSF_AT(float, sizeBytes, 0) =
        static_cast<float>(width) / static_cast<float>(g_ScreenWidth);
    CSF_AT(float, sizeBytes, 0x4) =
        static_cast<float>(height) / static_cast<float>(g_ScreenHeight);

    void* overlay = CSF_AT(void*, self, 0x104);
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, overlay, 0)[0x98 / 4])(
            overlay, nullptr, reinterpret_cast<float*>(sizeBytes));

    overlay = CSF_AT(void*, self, 0x104);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, overlay, 0)[0xA8 / 4])(overlay, nullptr, 0);

    overlay = CSF_AT(void*, self, 0x104);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, overlay, 0)[0x30 / 4])(
            overlay, nullptr, 1936994353, 1);
}

HOOK(0x0050E330, GameBddLoader_CreateScreenOverlay, "auto");
