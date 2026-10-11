#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, GfxScene_GetVisibilityController, 0x006BBFF0, ());
CSF_FUNC(int, __fastcall, GfxScene_InitializeVisibilityController, 0x006BC010, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GfxScene_GetVisibilityResources, 0x006B6AD0, ());
CSF_FUNC(uint32_t*, __fastcall, GfxScene_InitializeVisibilityResources, 0x006B6B00, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_LoadVisibilityResource, 0x006B7190, (void* self, void* edx, int a2));

static char __fastcall GfxScene_ConfigureVisibility(
    void* self, void* /*edx*/, const void* configuration)
{
    uint32_t* controller = GfxScene_GetVisibilityController();
    CSF_AT(uint32_t*, self, 0x98) = controller;
    GfxScene_InitializeVisibilityController(controller, nullptr);

    void* resources = GfxScene_GetVisibilityResources();
    CSF_AT(void*, self, 0x9C) = resources;
    GfxScene_InitializeVisibilityResources(resources, nullptr);

    const int visibilityData = CSF_AT(int, configuration, 0x04);
    if (!visibilityData)
        return 1;

    alignas(8) uint8_t descriptor[0xC] = {};
    CSF_AT(uint8_t, descriptor, 0x00) = CSF_AT(uint8_t, configuration, 0x00);
    CSF_AT(int, descriptor, 0x08) = CSF_AT(int, configuration, 0x20);
    CSF_AT(int, descriptor, 0x04) = visibilityData;

    void* visibilityController = CSF_AT(void*, self, 0x98);
    uint8_t success = static_cast<uint8_t>(
        ((int(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, visibilityController, 0)[0x28 / 4])(
                visibilityController, nullptr, descriptor)) & 1;

    visibilityController = CSF_AT(void*, self, 0x98);
    success &= static_cast<uint8_t>(
        ((int(__fastcall*)(void*, void*, uint8_t*, int))
            CSF_AT(void**, visibilityController, 0)[0x2C / 4])(
                visibilityController, nullptr, descriptor, 1));

    const char* resourceName = CSF_AT(const char*, configuration, 0x08);
    if (resourceName)
    {
        const char* end = resourceName;
        while (*end)
            ++end;

        if (end != resourceName)
        {
            success &= static_cast<uint8_t>(GfxScene_LoadVisibilityResource(
                CSF_AT(void*, self, 0x9C), nullptr,
                reinterpret_cast<int>(resourceName)));
        }
    }

    return static_cast<char>(success);
}

HOOK(0x006CAD20, GfxScene_ConfigureVisibility, "auto");
