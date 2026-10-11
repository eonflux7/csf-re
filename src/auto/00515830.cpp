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

// Callees (bound by address, signatures per the packet)
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));

// Data
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_profiles_savegames, 0x00899BC8);

// Vtable reference listed by the packet (no store in the original; kept as a no-op reference).
static void* const kS00692b80VtableRef = CSF_VTBL(0x00692B80);

// Walks the "NOMBRE profiles_savegames" property node: for every child, parses its
// integer component and applies it via SaveSlots_ApplyIndex. Returns the last result.
static S_00692b80* __stdcall ProfileSavegames_ApplyIndexList(S_00692b80* a1, S_004f21d0* a2)
{
    S_00692b80* result = nullptr;

    std::uint8_t* propertyNode = FindProperty(reinterpret_cast<std::uint8_t*>(a1), nullptr,
                                              const_cast<char*>(g_pszKey_NOMBRE_profiles_savegames));
    if (propertyNode)
    {
        std::uint32_t childCount = MenuExtras_GetChildCount(propertyNode, nullptr);
        result = reinterpret_cast<S_00692b80*>(childCount);
        std::uint32_t childIndex = 0;
        if (childCount)
        {
            do
            {
                unsigned char* component = reinterpret_cast<unsigned char*>(
                    GetComponent(propertyNode, nullptr, childIndex));
                int16_t slotIndex = static_cast<int16_t>(
                    ParseIntegerComponent(reinterpret_cast<std::uint32_t>(component)));
                result = reinterpret_cast<S_00692b80*>(
                    SaveSlots_ApplyIndex(a2, nullptr, slotIndex));
                ++childIndex;
            } while (childIndex < childCount);
        }
    }
    return result;
}

HOOK(0x515830, ProfileSavegames_ApplyIndexList, "auto");
