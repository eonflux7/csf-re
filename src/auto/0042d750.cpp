#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszKey_TEXTURE, 0x008995A4);
CSF_GLOBAL(const char*, g_pszKey_TOP, 0x008995AC);
CSF_GLOBAL(const char*, g_pszKey_LEFT, 0x008995B0);
CSF_GLOBAL(const char*, g_pszKey_WIDTH, 0x008995B4);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT, 0x008995B8);
CSF_GLOBAL(const char*, g_pszKey_TEX_V0, 0x008995BC);
CSF_GLOBAL(const char*, g_pszKey_TEX_U0, 0x008995C0);
CSF_GLOBAL(const char*, g_pszKey_TEX_V1, 0x008995C4);
CSF_GLOBAL(const char*, g_pszKey_TEX_U1, 0x008995C8);

static int __cdecl Hud_CreateTextureWidget(S_00692b80* properties, int layer)
{
    if (!properties)
        return 0;

    auto* propertyBytes = reinterpret_cast<std::uint8_t*>(properties);
    char textureName[256];
    MenuExtras_ReadText(propertyBytes, nullptr,
                       const_cast<char*>(g_pszKey_TEXTURE), textureName, 256);

    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* texturePaths =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[4])(accessor, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, texturePaths, 0)[4])(
            texturePaths, nullptr, CSF_STR(0x008308FC, "Gfx/Textures/"));

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* widget =
        reinterpret_cast<void*(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, controls, 0)[14])(controls, nullptr, textureName);

    if (widget)
    {
        int top = ReadIntegerProperty(propertyBytes, nullptr,
                                      const_cast<char*>(g_pszKey_TOP));
        int left = ReadIntegerProperty(propertyBytes, nullptr,
                                       const_cast<char*>(g_pszKey_LEFT));
        int width = ReadIntegerProperty(propertyBytes, nullptr,
                                        const_cast<char*>(g_pszKey_WIDTH));
        int height = ReadIntegerProperty(propertyBytes, nullptr,
                                         const_cast<char*>(g_pszKey_HEIGHT));
        float v0 = static_cast<float>(ReadFloatProperty(
            propertyBytes, nullptr, const_cast<char*>(g_pszKey_TEX_V0)));
        float u0 = static_cast<float>(ReadFloatProperty(
            propertyBytes, nullptr, const_cast<char*>(g_pszKey_TEX_U0)));
        float v1 = static_cast<float>(ReadFloatProperty(
            propertyBytes, nullptr, const_cast<char*>(g_pszKey_TEX_V1)));
        float u1 = static_cast<float>(ReadFloatProperty(
            propertyBytes, nullptr, const_cast<char*>(g_pszKey_TEX_U1)));

        int coordinates[2] = {left, top};
        reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, widget, 0)[10])(widget, nullptr, coordinates);

        coordinates[0] = width;
        coordinates[1] = height;
        reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, widget, 0)[14])(widget, nullptr, coordinates);

        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, widget, 0)[17])(widget, nullptr, layer);

        float textureCoordinates[4] = {u0, v0, u1, v1};
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, widget, 0)[19])(widget, nullptr, textureCoordinates);
    }

    return reinterpret_cast<int>(widget);
}

HOOK(0x0042D750, Hud_CreateTextureWidget, "auto");
