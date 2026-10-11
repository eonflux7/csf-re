#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(char, __fastcall, HudAlertIcons_ResetIconStyle, 0x004A0A60, (int* a1, int a2));
CSF_FUNC(const void*, __cdecl, RwImageSetPath, 0x00625270, (const void* path));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

using IconResourceArray = void*[2];
using FillResourceArray = void*[1];

CSF_GLOBAL(IconResourceArray, g_IconResources, 0x008B043C);
CSF_GLOBAL(FillResourceArray, g_FillResources, 0x008B0444);
CSF_GLOBAL(std::uint8_t, g_MenuResourcesPrepared, 0x008B0448);

static char __cdecl HudAlertIcons_PrepareMenuResources()
{
    const char prepared = static_cast<char>(g_MenuResourcesPrepared);
    if (prepared)
        return prepared;

    g_MenuResourcesPrepared = 1;

    void* fill = Mem_Alloc(48);
    if (fill)
    {
        CSF_AT(std::uint32_t, fill, 0x18) = 15;
        CSF_AT(std::uint32_t, fill, 0x14) = 0;
        CSF_AT(char, fill, 0x04) = 0;
        CSF_AT(std::uint32_t, fill, 0x1C) = 0;
        CSF_AT(float, fill, 0x20) = 0.0f;
        CSF_AT(float, fill, 0x24) = 0.0f;
        CSF_AT(float, fill, 0x28) = 1.0f;
        CSF_AT(float, fill, 0x2C) = 1.0f;
    }
    g_FillResources[0] = fill;

    for (unsigned int i = 0; i < 2; ++i)
    {
        void* icon = Mem_Alloc(140);
        if (icon)
        {
            CSF_AT(std::uint32_t, icon, 0x20) = 0;
            CSF_AT(std::uint32_t, icon, 0x24) = 15;
            CSF_AT(char, icon, 0x10) = 0;
            CSF_AT(std::uint32_t, icon, 0x40) = 15;
            CSF_AT(std::uint32_t, icon, 0x3C) = 0;
            CSF_AT(char, icon, 0x2C) = 0;
            CSF_AT(std::uint32_t, icon, 0x08) = 0;
            HudAlertIcons_ResetIconStyle(
                reinterpret_cast<int*>(static_cast<std::uint8_t*>(icon) + 8), 0);
            CSF_AT(std::uint32_t, icon, 0x00) = 0;
            CSF_AT(std::uint32_t, icon, 0x04) = 0;
        }
        g_IconResources[i] = icon;
    }

    alignas(8) std::uint8_t fileBytes[0x48];
    S_00692b10& file = *reinterpret_cast<S_00692b10*>(fileBytes);
    auto& iconGroup = CSF_AT(std::uint8_t*, fileBytes, 0x3C);
    auto& iconCount = CSF_AT(std::uint32_t, fileBytes, 0x40);
    auto& childIndexOrCount = CSF_AT(std::uint32_t, fileBytes, 0x44);

    (void)CSF_VTBL(0x00692B10);
    MenuExtras_InitializeFile(&file, nullptr);

    alignas(8) char text[0x100];
    std::memset(text, 0, sizeof(text));

    if (MenuExtras_LoadFile(&file, nullptr, CSF_STR(0x008343E8, "gfx/iconos.txt")))
    {
        RwImageSetPath(CSF_STR(0x008343D8, "gfx/textures/"));
        std::uint8_t* fills = FindProperty(
            reinterpret_cast<std::uint8_t*>(file.field_0000),
            nullptr, const_cast<char*>(CSF_STR(0x008343CC, ".RELLENOS")));

        if (fills)
        {
            (void)CSF_VTBL(0x00692B80);
            childIndexOrCount = MenuExtras_GetChildCount(fills, nullptr);
            for (unsigned int i = 0; i < childIndexOrCount; ++i)
            {
                auto* component = reinterpret_cast<std::uint8_t*>(
                    GetComponent(fills, nullptr, i));
                MenuExtras_ReadText(component, nullptr,
                    reinterpret_cast<char*>(CSF_ADDR_DATA(0x008305F4)), text, 255);

                if (!__stricmp(text, CSF_STR(0x0083439C, "PLANO")))
                {
                    MenuExtras_ReadText(component, nullptr,
                        const_cast<char*>(CSF_STR(0x008307BC, ".TEXTURE")), text, 255);

                    CSF_AT(float, g_FillResources[0], 0x20) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x0083078C, ".TEX_U0"))));
                    CSF_AT(float, g_FillResources[0], 0x24) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x00830794, ".TEX_V0"))));
                    CSF_AT(float, g_FillResources[0], 0x28) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x0083077C, ".TEX_U1"))));
                    CSF_AT(float, g_FillResources[0], 0x2C) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x00830784, ".TEX_V1"))));
                    AssignStringRange_429B60(g_FillResources[0], nullptr, text,
                        static_cast<unsigned int>(std::strlen(text)));
                }
            }

            std::uint8_t* icons = FindProperty(
                reinterpret_cast<std::uint8_t*>(file.field_0000),
                nullptr, const_cast<char*>(CSF_STR(0x008343C4, ".ICONOS")));
            iconGroup = icons;

            if (icons)
            {
                (void)CSF_VTBL(0x00692B80);
                iconCount = MenuExtras_GetChildCount(icons, nullptr);
                childIndexOrCount = 0;
                unsigned int i = 0;

                while (i < iconCount)
                {
                    auto* component = reinterpret_cast<std::uint8_t*>(
                        GetComponent(icons, nullptr, i));
                    MenuExtras_ReadText(component, nullptr,
                        reinterpret_cast<char*>(CSF_ADDR_DATA(0x008305F4)), text, 255);

                    int iconIndex = __stricmp(text, CSF_STR(0x00834394, "DUDOSO"));
                    if (iconIndex != 0)
                    {
                        if (__stricmp(text, CSF_STR(0x0083438C, "ALERTA")))
                        {
                            childIndexOrCount = ++i;
                            continue;
                        }
                        iconIndex = 1;
                    }

                    void* icon = g_IconResources[iconIndex];
                    MenuExtras_ReadText(component, nullptr,
                        const_cast<char*>(CSF_STR(0x00830690, ".RELLENO")), text, 255);

                    int fillIndex = -1;
                    if (!__stricmp(text, CSF_STR(0x0083439C, "PLANO")))
                        fillIndex = 0;
                    CSF_AT(int, icon, 0x04) = fillIndex;

                    MenuExtras_ReadText(component, nullptr,
                        const_cast<char*>(CSF_STR(0x008307BC, ".TEXTURE")), text, 255);
                    AssignStringRange_429B60(
                        static_cast<std::uint8_t*>(icon) + 12, nullptr, text,
                        static_cast<unsigned int>(std::strlen(text)));

                    CSF_AT(float, icon, 0x60) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x0083078C, ".TEX_U0"))));
                    CSF_AT(float, icon, 0x64) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x00830794, ".TEX_V0"))));
                    CSF_AT(float, icon, 0x68) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x0083077C, ".TEX_U1"))));
                    CSF_AT(float, icon, 0x6C) =
                        static_cast<float>(ReadFloatProperty(component, nullptr,
                            const_cast<char*>(CSF_STR(0x00830784, ".TEX_V1"))));

                    CSF_AT(std::uint32_t, icon, 0x08) = 3;
                    CSF_AT(float, icon, 0x80) = 25.0f;
                    CSF_AT(float, icon, 0x84) = 25.0f;
                    CSF_AT(std::uint32_t, icon, 0x88) =
                        CSF_AT(std::uint32_t,
                            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x2258);

                    fillIndex = CSF_AT(int, icon, 0x04);
                    void* selectedFill = g_FillResources[fillIndex];
                    char* fillTexture;
                    if (CSF_AT(std::uint32_t, selectedFill, 0x18) < 0x10u)
                        fillTexture = reinterpret_cast<char*>(selectedFill) + 4;
                    else
                        fillTexture = CSF_AT(char*, selectedFill, 0x04);

                    icons = iconGroup;
                    CSF_AT(char*, icon, 0x54) = fillTexture;
                    CSF_AT(std::uint32_t, icon, 0x70) =
                        CSF_AT(std::uint32_t, g_FillResources[fillIndex], 0x20);
                    CSF_AT(std::uint32_t, icon, 0x74) =
                        CSF_AT(std::uint32_t, g_FillResources[fillIndex], 0x24);
                    CSF_AT(std::uint32_t, icon, 0x78) =
                        CSF_AT(std::uint32_t, g_FillResources[fillIndex], 0x28);
                    CSF_AT(std::uint32_t, icon, 0x7C) =
                        CSF_AT(std::uint32_t, g_FillResources[fillIndex], 0x2C);

                    i = childIndexOrCount;
                    childIndexOrCount = ++i;
                }
            }
        }
    }

    return static_cast<char>(MenuExtras_CloseFile(&file, nullptr));
}

HOOK(0x004A0B10, HudAlertIcons_PrepareMenuResources, "auto");
