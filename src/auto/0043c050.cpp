#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* edx, int additionalCapacity));
CSF_FUNC(void, __cdecl, Hud_ReadRectFromProperties, 0x0042DF00, (S_00692b80* a1, int* a2));
CSF_FUNC(int, __cdecl, Hud_AppendWidgetHandle, 0x0042FF30, (S_00692b80* a1, int a2, S_0041e5f0* a3));
CSF_FUNC(void, __fastcall, Minimap_SetupFromComponent, 0x0043BB40, (void* self, void* edx, float a3));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

static char __fastcall MinimapRadar_LoadFromProperties(void* self, void* /*edx*/, S_00692b80* a2)
{
    std::uint8_t* me = reinterpret_cast<std::uint8_t*>(self);

    CSF_AT(std::uint8_t, me, 8) = 0;
    CSF_AT(std::uint8_t, me, 0x4C) = 0;

    std::uint8_t* prop = FindProperty(reinterpret_cast<std::uint8_t*>(a2), nullptr,
                                      const_cast<char*>(CSF_STR(0x00830E30, ".MINIMAPA_RADAR_MP")));
    if (!prop)
        return CSF_AT(std::uint8_t, me, 8);

    if (MenuExtras_GetChildCount(prop, nullptr) != 16) {
        MenuExtras_GetChildCount(prop, nullptr);
        return CSF_AT(std::uint8_t, me, 8);
    }

    std::uint32_t comp0 = GetComponent(prop, nullptr, 0);
    if (!comp0)
        return 0;
    Hud_ReadRectFromProperties(reinterpret_cast<S_00692b80*>(static_cast<std::uintptr_t>(comp0)),
                               reinterpret_cast<int*>(me + 0x24));

    std::uint32_t comp1 = GetComponent(prop, nullptr, 1);
    if (!comp1)
        return 0;

    float comp1Bits;
    std::memcpy(&comp1Bits, &comp1, sizeof(comp1Bits));
    Minimap_SetupFromComponent(me, nullptr, comp1Bits);

    std::uint32_t capacity = CSF_AT(std::uint32_t, me, 0x10);
    S_0041e5f0* handles = reinterpret_cast<S_0041e5f0*>(me + 0xC);
    if (capacity < 0xE)
        S_0041e5f0_GrowArrayCapacity(handles, nullptr, 14 - static_cast<int>(capacity));

    std::uint32_t comp2 = GetComponent(prop, nullptr, 2);
    int appended = Hud_AppendWidgetHandle(reinterpret_cast<S_00692b80*>(static_cast<std::uintptr_t>(comp2)), 4, handles);

    std::uint32_t index = 3;
    while (appended != 0) {
        std::uint32_t comp = GetComponent(prop, nullptr, index);
        appended = Hud_AppendWidgetHandle(reinterpret_cast<S_00692b80*>(static_cast<std::uintptr_t>(comp)), 4, handles);
        ++index;
        if (index >= 0x10) {
            if (appended != 0) {
                void* target = CSF_AT(void*, CSF_AT(std::uint8_t*, handles, 0), 0x34);
                void** vtbl = CSF_AT(void**, target, 0);
                ((void(__fastcall*)(void*, void*, int))vtbl[0x70 / 4])(target, nullptr, 1);
            }
            break;
        }
    }

    CSF_AT(std::uint8_t, me, 8) = (appended != 0) ? 1 : 0;
    return CSF_AT(std::uint8_t, me, 8);
}

HOOK(0x0043C050, MinimapRadar_LoadFromProperties, "auto");
