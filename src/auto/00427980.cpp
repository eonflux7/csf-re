#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));

static std::uint32_t* __fastcall HudObjectives_LoadImageResource(
    void* self,
    void* /*edx*/,
    S_00692b80* properties,
    S_00692b80* propertyName,
    const char* fallback)
{
    char resource[256] = {};
    char* keyword = reinterpret_cast<char*>(propertyName);

    if (properties &&
        S_00692b80_FindProperty(properties, nullptr, keyword))
    {
        MenuExtras_ReadText(
            reinterpret_cast<std::uint8_t*>(properties),
            nullptr, keyword, resource, 256);
    }
    else
    {
        std::strcpy(resource, fallback);
    }

    std::uint8_t resourcePath[256] = {};
    char resourceName[256] = {};

    Hud_ExtractResourcePath(resource, resourcePath, 256);
    NormalizeResourceName(resource, resourceName, 256);
    RwImageSetPath(resourcePath);

    void* imageManager = CSF_AT(void*, self, 0x4);
    auto* image =
        reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, imageManager, 0)[0x38 / 4])(
                imageManager, nullptr, resourceName);

    if (image)
    {
        alignas(8) std::uint8_t rectangleBytes[0x10];
        CSF_AT(float, rectangleBytes, 0x0) = 0.0f;
        CSF_AT(float, rectangleBytes, 0x4) = 0.0f;
        CSF_AT(float, rectangleBytes, 0x8) = 1.0f;
        CSF_AT(float, rectangleBytes, 0xC) = 1.0f;

        reinterpret_cast<void (__fastcall*)(std::uint32_t*, void*, float*)>(
            CSF_AT(void**, image, 0)[0x4C / 4])(
                image, nullptr, reinterpret_cast<float*>(rectangleBytes));
        return image;
    }

    CSF_AT(std::uint8_t, self, 0x8) = 0;
    return nullptr;
}

HOOK(0x00427980, HudObjectives_LoadImageResource, "auto");
