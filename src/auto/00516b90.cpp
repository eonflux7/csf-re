#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, sub_511D10, 0x00511D10, (void* a1, int a2));
CSF_FUNC(void*, __stdcall, sub_515830, 0x00515830, (void* a1, void* a2));
CSF_FUNC(char, __stdcall, sub_5165A0, 0x005165A0, (void* a5, int a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_KEY, 0x00899BCC);
CSF_GLOBAL(const char*, g_pszKey_CURRENT, 0x00899BD0);
CSF_GLOBAL(const char*, g_pszKey_CAMPA, 0x00899BD4);
CSF_GLOBAL(const char*, g_pszKey_COMMANDO_DIF, 0x00899BE8);

static int __stdcall ProfilesSavegames_ReadCampaignEntry(
    S_00692b80* properties, int destinationAddress, int version)
{
    if (!properties)
        return 0;

    auto* destination = reinterpret_cast<std::uint8_t*>(destinationAddress);
    auto* propertyBytes = reinterpret_cast<std::uint8_t*>(properties);

    sub_515830(properties, destination);

    CSF_AT(int, destination, 0x48) =
        ReadIntegerProperty(propertyBytes, nullptr, const_cast<char*>(g_pszKey_KEY));

    CSF_AT(std::uint8_t, destination, 0x4C) =
        ReadIntegerProperty(propertyBytes, nullptr, const_cast<char*>(g_pszKey_CURRENT)) == 1;

    CSF_AT(std::uint8_t, destination, 0x44) =
        ReadIntegerProperty(propertyBytes, nullptr, const_cast<char*>(g_pszKey_COMMANDO_DIF)) == 1;

    if (sub_511D10(properties, reinterpret_cast<int>(destination + 0x1C)))
    {
        auto* campaign = FindProperty(
            propertyBytes, nullptr, const_cast<char*>(g_pszKey_CAMPA));

        if (version >= 12 && version <= 13 &&
            sub_5165A0(campaign, destinationAddress))
        {
            return 1;
        }
    }

    return 0;
}

HOOK(0x00516B90, ProfilesSavegames_ReadCampaignEntry, "auto");
