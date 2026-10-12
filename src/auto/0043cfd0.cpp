#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_AppendWidgetHandle, 0x0042FF30, (S_00692b80* a1, int a2, S_0041e5f0* a3));
CSF_FUNC(char, __fastcall, MinimapRadar_ValidateProperty, 0x0043CEF0, (void* self, void* edx, void* a2));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_MINIMAPA_RTS, 0x00899694);

static char __fastcall C_00830e5c_LoadMinimapWidgets(C_00830e5c* self, void* /*edx*/, S_00692b80* a2)
{
    self->field_0008 = 0;

    std::uint8_t* property = FindProperty(reinterpret_cast<std::uint8_t*>(a2), nullptr,
                                          const_cast<char*>(g_pszKey_MINIMAPA_RTS));
    if (!property)
        return static_cast<char>(self->field_0008);

    if (MenuExtras_GetChildCount(property, nullptr) == 19)
    {
        char ok = MinimapRadar_ValidateProperty(self, nullptr, property);
        if (!ok)
            return ok;

        unsigned int index = 2;
        S_0041e5f0* handles = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<std::uint8_t*>(self) + 0xC);
        while (true)
        {
            std::uint32_t component = GetComponent(property, nullptr, index);
            if (!Hud_AppendWidgetHandle(reinterpret_cast<S_00692b80*>(component), 4, handles))
                break;
            ++index;
            if (index >= 0x13)
            {
                self->field_0008 = 1;
                break;
            }
        }
        return static_cast<char>(self->field_0008);
    }

    MenuExtras_GetChildCount(property, nullptr);
    return static_cast<char>(self->field_0008);
}

HOOK(0x0043CFD0, C_00830e5c_LoadMinimapWidgets, "auto");
