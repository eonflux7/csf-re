#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(std::uint32_t, g_PreviewCameraAngleBits, 0x00860DAC);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static char __fastcall DevConsole_CreateRenderResources(void* self, void* /*edx*/)
{
    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const int width =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, display, 0)[0x20 / 4])(
            display, nullptr);

    display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    const int height =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, display, 0)[0x24 / 4])(
            display, nullptr);

    const int originX = static_cast<int>(static_cast<double>(width) * g_fZero);
    const float heightAsFloat = static_cast<float>(height);
    const int originY = static_cast<int>(static_cast<double>(height) * g_fZero);

    alignas(8) std::uint8_t viewportBytes[0x28]{};
    alignas(8) std::uint8_t cameraBytes[0x34]{};
    alignas(8) std::uint8_t lightBytes[0x10]{};

    CSF_AT(int, cameraBytes, 0x28) = originY;
    CSF_AT(int, cameraBytes, 0x2C) = width;

    CSF_AT(int, viewportBytes, 0x10) = static_cast<int>(heightAsFloat);
    CSF_AT(int, viewportBytes, 0x08) = originY;
    CSF_AT(int, viewportBytes, 0x0C) = width;
    CSF_AT(float, viewportBytes, 0x14) = g_ColorBlack;
    CSF_AT(int, viewportBytes, 0x00) = 7;
    CSF_AT(std::uint8_t, viewportBytes, 0x24) = 1;
    CSF_AT(std::uint8_t, viewportBytes, 0x25) = 0;
    CSF_AT(int, viewportBytes, 0x04) = originX;
    CSF_AT(float, viewportBytes, 0x18) = g_ColorBlack_G;
    CSF_AT(float, viewportBytes, 0x1C) = g_ColorBlack_B;
    CSF_AT(float, viewportBytes, 0x20) = g_ColorBlack_A;

    void* manager = reinterpret_cast<void*>(
        ResolveRwManager(GetGlobalInt(), nullptr));
    CSF_AT(void*, self, 0x04) =
        ((void*(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, manager, 0)[0x18 / 4])(
                manager, nullptr, viewportBytes);

    CSF_AT(std::uint32_t, cameraBytes, 0x00) = 0;
    CSF_AT(std::uint32_t, cameraBytes, 0x04) = 0;
    CSF_AT(std::uint32_t, cameraBytes, 0x08) = 0;
    CSF_AT(std::uint32_t, cameraBytes, 0x0C) = 0;
    CSF_AT(std::uint32_t, cameraBytes, 0x20) = 0;
    CSF_AT(std::uint32_t, cameraBytes, 0x18) = 0;
    CSF_AT(std::uint32_t, cameraBytes, 0x1C) = g_PreviewCameraAngleBits;
    CSF_AT(std::uint32_t, cameraBytes, 0x10) = 1101004800;
    CSF_AT(std::uint32_t, cameraBytes, 0x14) = 1148846080;

    manager = reinterpret_cast<void*>(
        ResolveRwManager(GetGlobalInt(), nullptr));
    void* camera =
        ((void*(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, manager, 0)[0x10 / 4])(
                manager, nullptr, cameraBytes);

    void* viewport = CSF_AT(void*, self, 0x04);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, viewport, 0)[0x10 / 4])(viewport, nullptr, camera);

    viewport = CSF_AT(void*, self, 0x04);
    ((void(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, viewport, 0)[0x4C / 4])(viewport, nullptr, 1, 0);

    viewport = CSF_AT(void*, self, 0x04);
    ((void(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, viewport, 0)[0x4C / 4])(viewport, nullptr, 2, 0);

    viewport = CSF_AT(void*, self, 0x04);
    ((void(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, viewport, 0)[0x4C / 4])(viewport, nullptr, 4, 0);

    CSF_AT(int, lightBytes, 0x04) = 0;
    CSF_AT(int, lightBytes, 0x00) = 2;
    CSF_AT(int, lightBytes, 0x08) = height;
    CSF_AT(float, lightBytes, 0x0C) = heightAsFloat;

    manager = reinterpret_cast<void*>(
        ResolveRwManager(GetGlobalInt(), nullptr));
    void* light =
        ((void*(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, manager, 0)[0x20 / 4])(
                manager, nullptr, lightBytes);
    CSF_AT(void*, self, 0x08) = light;

    if (light)
    {
        viewport = CSF_AT(void*, self, 0x04);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, viewport, 0)[0x30 / 4])(viewport, nullptr, light);
    }
    return 1;
}

HOOK(0x004FBA30, DevConsole_CreateRenderResources, "auto");
