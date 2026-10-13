#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_CreateTextureWidget, 0x0042D750, (S_00692b80* properties, int layer));
CSF_FUNC(void*, __fastcall, S_0041e5f0_EnsureButtonCapacity, 0x0042FB10, (S_0041e5f0* self, void* , void* requestedCount));
CSF_FUNC(int, __cdecl, Hud_AppendWidgetHandle, 0x0042FF30, (S_00692b80* a1, int a2, S_0041e5f0* a3));
CSF_FUNC(void, __fastcall, HudSpritesLayout_LoadTacticalMapBase, 0x00436790, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, HudSpritesLayout_LoadTacticalMapOverlay, 0x00436840, (void* self, void* edx, void* a2));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_SPRITES_CGuiControl, 0x00899620);
CSF_GLOBAL(const char*, g_pszKey_SPRITES_MINIMAPA_CGuiControl, 0x00899624);
CSF_GLOBAL(const char*, g_pszKey_MAPA_TACTICO, 0x0089962C);
CSF_GLOBAL(const char*, g_pszKey_SANITARIO, 0x00899630);

static char __fastcall HudSpritesLayout_LoadMinimapWidgets(
    void* self, void* /*edx*/, S_00692b80* properties)
{
    // The packet also identifies the child-count entry as a vtable address.
    (void)CSF_VTBL(0x00692B80);

    const char initialized = CSF_AT(char, self, 0x15C);
    if (initialized)
        return initialized;

    CSF_AT(std::uint32_t, self, 0x168) = 0;

    auto* sprites = FindProperty(
        reinterpret_cast<std::uint8_t*>(properties), nullptr,
        const_cast<char*>(g_pszKey_SPRITES_CGuiControl));
    if (!sprites)
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(sprites));

    auto* minimap = FindProperty(
        sprites, nullptr, const_cast<char*>(g_pszKey_SPRITES_MINIMAPA_CGuiControl));
    if (!minimap)
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(minimap));

    auto* tacticalMap = FindProperty(
        minimap, nullptr, const_cast<char*>(g_pszKey_MAPA_TACTICO));
    if (tacticalMap)
    {
        if (MenuExtras_GetChildCount(tacticalMap, nullptr) == 27)
        {
            auto component = GetComponent(tacticalMap, nullptr, 0);
            HudSpritesLayout_LoadTacticalMapBase(
                self, nullptr, reinterpret_cast<void*>(component));

            component = GetComponent(tacticalMap, nullptr, 1u);
            HudSpritesLayout_LoadTacticalMapOverlay(
                self, nullptr, reinterpret_cast<void*>(component));

            auto* widgets = reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x160);
            S_0041e5f0_EnsureButtonCapacity(
                widgets, nullptr, reinterpret_cast<void*>(0x17));

            unsigned int index = 2;
            for (;;)
            {
                component = GetComponent(tacticalMap, nullptr, index);
                if (!Hud_AppendWidgetHandle(
                        reinterpret_cast<S_00692b80*>(component), 4, widgets))
                    break;

                if (++index >= 0x1B)
                {
                    CSF_AT(char, self, 0x15C) = 1;
                    break;
                }
            }
        }
        else
        {
            MenuExtras_GetChildCount(tacticalMap, nullptr);
        }
    }

    auto* medical = FindProperty(
        minimap, nullptr, const_cast<char*>(g_pszKey_SANITARIO));
    if (!medical)
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(medical));

    const std::uint32_t childCount = MenuExtras_GetChildCount(medical, nullptr);
    if (childCount != 1)
        return static_cast<char>(childCount);

    const auto component = GetComponent(medical, nullptr, 0);
    const int widget = Hud_CreateTextureWidget(
        reinterpret_cast<S_00692b80*>(component), 4);
    CSF_AT(std::uint32_t, self, 0x170) = static_cast<std::uint32_t>(widget);
    return static_cast<char>(widget);
}

HOOK(0x00437A50, HudSpritesLayout_LoadMinimapWidgets, "auto");
