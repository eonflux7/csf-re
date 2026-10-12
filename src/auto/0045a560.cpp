#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global holding the const char* key ".ICONOS_ESTADO" (value at 0x899740).
CSF_GLOBAL(char*, g_pszKey_ICONOS_ESTADO, 0x00899740);

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __cdecl, Hud_TryReadIconProperty, 0x0042E640, (std::uint8_t* hudObject, int* iconOut, int* extraOut));
CSF_FUNC(uint32_t*, __fastcall, HudStatusIcons_ApplyIcon, 0x0045A0C0, (void* self, void* edx, uint32_t* a3));

static void __fastcall C_008313dc_ProcessStatusIcons(C_008313dc* self, void* /*edx*/, S_00692b80* sceneRoot)
{
    unsigned int index = 0;

    if (sceneRoot == nullptr)
        return;

    std::uint8_t* iconsNode = FindProperty(reinterpret_cast<std::uint8_t*>(sceneRoot), nullptr, g_pszKey_ICONOS_ESTADO);
    if (iconsNode == nullptr)
        return;

    unsigned int childCount = MenuExtras_GetChildCount(iconsNode, nullptr);

    int iconIndex = -1;
    int extraValues[4];
    std::memset(extraValues, 0, sizeof(extraValues));

    if (childCount != 0)
    {
        do
        {
            std::uint8_t* component = reinterpret_cast<std::uint8_t*>(GetComponent(iconsNode, nullptr, index));
            if (Hud_TryReadIconProperty(component, &iconIndex, extraValues))
            {
                HudStatusIcons_ApplyIcon(reinterpret_cast<std::uint8_t*>(self) + 0x28, nullptr,
                                         reinterpret_cast<uint32_t*>(&iconIndex));
            }
            ++index;
        } while (index < childCount);
    }
}

HOOK(0x0045A560, C_008313dc_ProcessStatusIcons, "auto");
