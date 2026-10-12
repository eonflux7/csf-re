#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));
CSF_FUNC(void, __cdecl, Hud_ReadRectFromProperties, 0x0042DF00, (S_00692b80* a1, int* a2));
CSF_FUNC(int, __cdecl, Hud_AppendWidgetHandle, 0x0042FF30, (S_00692b80* a1, int a2, S_0041e5f0* a3));
CSF_FUNC(void, __fastcall, Minimap_ParseRadarLayout, 0x0043C3A0, (void* self, void* edx, float a3));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_MINIMAPA_RADAR, 0x00899690);

static char __fastcall Minimap_LoadRadarProperties(void* self, void* /*edx*/, S_00692b80* config)
{
    CSF_AT(std::uint8_t, self, 8) = 0;

    std::uint8_t* radar = FindProperty(reinterpret_cast<std::uint8_t*>(config), nullptr,
                                       const_cast<char*>(g_pszKey_MINIMAPA_RADAR));
    if (!radar)
        return CSF_AT(std::uint8_t, self, 8);

    if (MenuExtras_GetChildCount(radar, nullptr) != 15) {
        MenuExtras_GetChildCount(radar, nullptr);
        return CSF_AT(std::uint8_t, self, 8);
    }

    std::uint32_t comp = GetComponent(radar, nullptr, 0);
    if (!comp)
        return 0;
    Hud_ReadRectFromProperties(reinterpret_cast<S_00692b80*>(comp),
                               reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self) + 0x24));

    comp = GetComponent(radar, nullptr, 1);
    if (!comp)
        return 0;
    float compAsFloat;
    std::memcpy(&compAsFloat, &comp, sizeof(compAsFloat));
    Minimap_ParseRadarLayout(self, nullptr, compAsFloat);

    std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x10);
    S_0041e5f0* widgets = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<std::uint8_t*>(self) + 0xC);
    if (capacity < 0xD)
        S_0041e5f0_GrowArrayCapacity(widgets, nullptr, 13 - static_cast<int>(capacity));

    comp = GetComponent(radar, nullptr, 2);
    int appended = Hud_AppendWidgetHandle(reinterpret_cast<S_00692b80*>(comp), 4, widgets);
    unsigned int index = 3;
    while (appended != 0) {
        comp = GetComponent(radar, nullptr, index);
        appended = Hud_AppendWidgetHandle(reinterpret_cast<S_00692b80*>(comp), 4, widgets);
        ++index;
        if (index >= 0xF) {
            if (appended != 0) {
                std::uint32_t base = CSF_AT(std::uint32_t, widgets, 0);
                void* obj = CSF_AT(void*, reinterpret_cast<void*>(base), 40);
                void** vtbl = CSF_AT(void**, obj, 0);
                ((void(__fastcall*)(void*, void*, int))vtbl[0x70 / 4])(obj, nullptr, 1);
            }
            break;
        }
    }

    CSF_AT(std::uint8_t, self, 8) = (appended != 0) ? 1 : 0;
    return CSF_AT(std::uint8_t, self, 8);
}

HOOK(0x0043CB10, Minimap_LoadRadarProperties, "auto");
