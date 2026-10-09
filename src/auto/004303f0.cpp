#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_ParseComponentValue, 0x0042D750, (void* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, Hud_ResizeComponentArray, 0x0042FB30, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_POS, 0x00899568);

static void __fastcall Hud_LoadWidgetPositions(void* self, void* /*edx*/, S_00692b80* definition)
{
    (void)CSF_VTBL(0x00692B80);
    CSF_AT(std::uint32_t, self, 0x7C) = 0;

    if (!definition)
    {
        CSF_AT(std::uint8_t, self, 0x127) = 0;
        return;
    }

    if (MenuExtras_GetChildCount(definition, nullptr) != 4)
    {
        MenuExtras_GetChildCount(definition, nullptr);
        CSF_AT(std::uint8_t, self, 0x127) = 0;
        return;
    }

    Hud_ResizeComponentArray(static_cast<std::uint8_t*>(self) + 0x74, nullptr, 4u, 0);

    auto* component = reinterpret_cast<std::uint8_t*>(
        GetComponent(reinterpret_cast<unsigned char*>(definition), nullptr, 0));
    if (component)
    {
        auto* position = FindProperty(component, nullptr, const_cast<char*>(g_pszKey_POS));
        if (position && MenuExtras_GetChildCount(position, nullptr) == 2)
        {
            auto value = GetComponent(position, nullptr, 0);
            CSF_AT(int, self, 0xF0) = ParseIntegerComponent(value);
            value = GetComponent(position, nullptr, 1u);
            CSF_AT(int, self, 0xF4) = ParseIntegerComponent(value);
        }
    }

    component = reinterpret_cast<std::uint8_t*>(
        GetComponent(reinterpret_cast<unsigned char*>(definition), nullptr, 1u));
    if (component)
    {
        auto* position = FindProperty(component, nullptr, const_cast<char*>(g_pszKey_POS));
        if (position && MenuExtras_GetChildCount(position, nullptr) == 2)
        {
            auto value = GetComponent(position, nullptr, 0);
            CSF_AT(int, self, 0xF8) = ParseIntegerComponent(value);
            value = GetComponent(position, nullptr, 1u);
            CSF_AT(int, self, 0xFC) = ParseIntegerComponent(value);
        }
    }

    auto* destination = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x74)) + 0xC;
    auto* finalComponent = reinterpret_cast<void*>(
        GetComponent(reinterpret_cast<unsigned char*>(definition), nullptr, 3u));
    CSF_AT(int, destination, 0) = Hud_ParseComponentValue(finalComponent, 0);
    CSF_AT(std::uint8_t, self, 0x127) = 1;
}

HOOK(0x004303F0, Hud_LoadWidgetPositions, "auto");
