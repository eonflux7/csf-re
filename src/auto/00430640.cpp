#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Hud_FindTextureSlot, 0x0042D8B0, (char* String1));
CSF_FUNC(unsigned int, __fastcall, Hud_ResizeTextureSlots, 0x0042FB30, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszKey_TEXTURE, 0x008995A4);
CSF_GLOBAL(const char*, g_pszKey_ID_hud, 0x008995A8);
CSF_GLOBAL(const char*, g_pszKey_TOP, 0x008995AC);
CSF_GLOBAL(const char*, g_pszKey_LEFT, 0x008995B0);
CSF_GLOBAL(const char*, g_pszKey_WIDTH, 0x008995B4);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT, 0x008995B8);
CSF_GLOBAL(const char*, g_pszKey_TEX_V0, 0x008995BC);
CSF_GLOBAL(const char*, g_pszKey_TEX_U0, 0x008995C0);
CSF_GLOBAL(const char*, g_pszKey_TEX_V1, 0x008995C4);
CSF_GLOBAL(const char*, g_pszKey_TEX_U1, 0x008995C8);

static void __fastcall Hud_LoadTextureWidgets(char* self, void* /*edx*/, S_00692b80* node)
{
    std::uint32_t position[3];
    position[2] = reinterpret_cast<std::uint32_t>(self);

    if (!node)
        return;

    char text[256] = {};

    // The packet also labels this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x00692B80);
    const unsigned int childCount = MenuExtras_GetChildCount(node, nullptr);
    auto* textureSlots = reinterpret_cast<S_0041e5f0*>(self + 0x1E8);
    Hud_ResizeTextureSlots(textureSlots, nullptr, childCount, 0);

    for (unsigned int index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, index));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_ID_hud), text, 256);
        const int slot = Hud_FindTextureSlot(text);
        if (slot == -1)
            continue;

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_TEXTURE), text, 256);

        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
        void* textureManager =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, accessor, 0)[4])(accessor, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, textureManager, 0)[4])(
                textureManager, nullptr, CSF_STR(0x008308FC, "Gfx/Textures/"));

        global = reinterpret_cast<void*>(GetGlobalInt());
        void* controls = MenuControls_ResolveObject(global, nullptr);
        void* texture = reinterpret_cast<void*(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, controls, 0)[14])(controls, nullptr, text);
        reinterpret_cast<void**>(textureSlots->field_0000)[slot] = texture;

        const int top = ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_TOP));
        const int left = ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_LEFT));
        const int width = ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_WIDTH));
        const int height = ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_HEIGHT));
        const float v0 = static_cast<float>(
            ReadFloatProperty(component, nullptr, const_cast<char*>(g_pszKey_TEX_V0)));
        const float u0 = static_cast<float>(
            ReadFloatProperty(component, nullptr, const_cast<char*>(g_pszKey_TEX_U0)));
        const float v1 = static_cast<float>(
            ReadFloatProperty(component, nullptr, const_cast<char*>(g_pszKey_TEX_V1)));
        const float u1 = static_cast<float>(
            ReadFloatProperty(component, nullptr, const_cast<char*>(g_pszKey_TEX_U1)));

        texture = reinterpret_cast<void**>(textureSlots->field_0000)[slot];
        if (!texture)
            continue;

        position[0] = static_cast<std::uint32_t>(left);
        position[1] = static_cast<std::uint32_t>(top);
        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, texture, 0)[10])(texture, nullptr, position);

        texture = reinterpret_cast<void**>(textureSlots->field_0000)[slot];
        int size[2] = {width, height};
        reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, texture, 0)[14])(texture, nullptr, size);

        texture = reinterpret_cast<void**>(textureSlots->field_0000)[slot];
        float coordinates[4] = {u0, v0, u1, v1};
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, texture, 0)[19])(texture, nullptr, coordinates);

        texture = reinterpret_cast<void**>(textureSlots->field_0000)[slot];
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, texture, 0)[17])(texture, nullptr, 0);
    }
}

HOOK(0x00430640, Hud_LoadTextureWidgets, "auto");
