#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(unsigned int, __cdecl, SceneLoader_CopyMapFilename, 0x006948E0, (char* a1, char* a2, unsigned int a3));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(const char*, g_pszKey_TEXTURA, 0x008A8640);
CSF_GLOBAL(const char*, g_pszKey_RASTER, 0x008A8644);
CSF_GLOBAL(const char*, g_pszKey_RENDER, 0x008A864C);
CSF_GLOBAL(const char*, g_pszKey_FLIP, 0x008A8650);
CSF_GLOBAL(const char*, g_pszKey_U1, 0x008A8654);
CSF_GLOBAL(const char*, g_pszKey_U2, 0x008A8658);
CSF_GLOBAL(const char*, g_pszKey_V1, 0x008A865C);
CSF_GLOBAL(const char*, g_pszKey_V2, 0x008A8660);

static int __fastcall CGuiBuilder_CreateTextureQuad(
    CGuiBuilder* self, void* /*edx*/, S_00692b80* properties)
{
    if (!properties)
        return 0;

    auto* node = reinterpret_cast<std::uint8_t*>(properties);
    char resourceName[0x100];
    char destination[0x100];
    char resourcePath[0x100];
    std::memset(resourceName, 0, sizeof(resourceName));

    int textureQuad = 0;
    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_TEXTURA)))
    {
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_TEXTURA),
                           resourceName, 256);
        std::memset(resourcePath, 0, sizeof(resourcePath));
        std::memset(destination, 0, sizeof(destination));

        Hud_ExtractResourcePath(
            resourceName, reinterpret_cast<std::uint8_t*>(resourcePath), 256);
        NormalizeResourceName(resourceName, destination, 256);
        RwImageSetPath(resourcePath);

        void* factory = reinterpret_cast<void*>(self->field_0004);
        textureQuad = ((int(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, factory, 0)[56 / 4])(factory, nullptr, destination);

        if (textureQuad)
        {
            if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_U1)))
            {
                alignas(8) std::uint8_t coordinates[0x14];
                const float u1 = static_cast<float>(
                    ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_U1)));
                const float u2 = static_cast<float>(
                    ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_U2)));
                CSF_AT(float, coordinates, 0x4) = static_cast<float>(
                    ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_V1)));
                CSF_AT(float, coordinates, 0x0) = u1;
                CSF_AT(float, coordinates, 0x8) = u2;
                CSF_AT(float, coordinates, 0xC) = static_cast<float>(
                    ReadFloatAttribute(node, nullptr, const_cast<char*>(g_pszKey_V2)));

                void* quad = reinterpret_cast<void*>(textureQuad);
                ((void(__fastcall*)(void*, void*, float*))
                    CSF_AT(void**, quad, 0)[76 / 4])(
                        quad, nullptr, reinterpret_cast<float*>(coordinates));
            }

            if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_RENDER)))
            {
                MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_RENDER),
                                   resourceName, 256);
                if (__stricmp(resourceName, CSF_STR(0x00864B7C, "ADITIVO")) == 0)
                {
                    void* quad = reinterpret_cast<void*>(textureQuad);
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, quad, 0)[112 / 4])(quad, nullptr, 1);
                }
            }

            auto* flipProperty =
                FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FLIP));
            if (flipProperty)
            {
                void* quad = reinterpret_cast<void*>(textureQuad);
                void** quadVtable = CSF_AT(void**, quad, 0);
                const int flip = S_00692b80_ParseIntegerComponent(
                    reinterpret_cast<S_00692b80*>(flipProperty), nullptr) != 0;
                ((void(__fastcall*)(void*, void*, int))
                    quadVtable[120 / 4])(quad, nullptr, flip);
                return textureQuad;
            }
        }
    }
    else if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_RASTER)))
    {
        MenuExtras_ReadText(node, nullptr, const_cast<char*>(g_pszKey_RASTER),
                           resourceName, 256);
        std::memset(destination, 0, sizeof(destination));
        std::memset(resourcePath, 0, sizeof(resourcePath));

        Hud_ExtractResourcePath(
            resourceName, reinterpret_cast<std::uint8_t*>(destination), 256);
        SceneLoader_CopyMapFilename(resourceName, resourcePath, 256);
        RwImageSetPath(destination);

        void* factory = reinterpret_cast<void*>(self->field_0004);
        return ((int(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, factory, 0)[64 / 4])(factory, nullptr, resourcePath);
    }

    return textureQuad;
}

HOOK(0x0071E4E0, CGuiBuilder_CreateTextureQuad, "auto");
