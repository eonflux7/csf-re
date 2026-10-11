#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszKey_NOMBRE_profiles_savegames, 0x00899BC8);
CSF_GLOBAL(const char*, g_pszKey_KEY, 0x00899BCC);
CSF_GLOBAL(const char*, g_pszKey_CURRENT, 0x00899BD0);
CSF_GLOBAL(const char*, g_pszKey_CAMPA, 0x00899BD4);
CSF_GLOBAL(const char*, g_pszKey_COMMANDO_DIF, 0x00899BE8);

CSF_FUNC(char, __cdecl, ProfilesSavegames_ReadProfileDetails, 0x00511D10, (void* a1, int a2));
CSF_FUNC(char, __stdcall, ProfilesSavegames_ReadCampaignData, 0x005165A0, (void* a5, int a2));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(uint8_t*, __fastcall, Property_ReadWideText, 0x006939F0, (void* self, void* edx, char* String1, int64_t a3));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));

static int __stdcall ProfilesSavegames_ReadProfileEntry(
    S_00692b80* properties, S_004f21d0* entry, int version)
{
    alignas(8) std::uint8_t nameBytes[0x200];
    auto* name = reinterpret_cast<std::int16_t*>(nameBytes);
    name[0] = 0;

    if (!properties)
        return 0;

    const std::int64_t textBufferArgument = static_cast<std::int64_t>(
        (std::uint64_t{256} << 32) |
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(name)));

    Property_ReadWideText(
        properties, nullptr,
        const_cast<char*>(g_pszKey_NOMBRE_profiles_savegames),
        textBufferArgument);

    MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(entry), nullptr, name);

    entry->field_0048 = ReadIntegerProperty(
        reinterpret_cast<std::uint8_t*>(properties), nullptr,
        const_cast<char*>(g_pszKey_KEY));

    *reinterpret_cast<std::uint8_t*>(&entry->field_004c) =
        ReadIntegerProperty(
            reinterpret_cast<std::uint8_t*>(properties), nullptr,
            const_cast<char*>(g_pszKey_CURRENT)) == 1;

    *reinterpret_cast<std::uint8_t*>(&entry->field_0044) =
        ReadIntegerProperty(
            reinterpret_cast<std::uint8_t*>(properties), nullptr,
            const_cast<char*>(g_pszKey_COMMANDO_DIF)) == 1;

    if (!ProfilesSavegames_ReadProfileDetails(
            properties,
            static_cast<int>(
                reinterpret_cast<std::uintptr_t>(&entry->field_001c))))
        return 0;

    auto* campaign = FindProperty(
        reinterpret_cast<std::uint8_t*>(properties), nullptr,
        const_cast<char*>(g_pszKey_CAMPA));

    if (version >= 12 && version <= 13 &&
        ProfilesSavegames_ReadCampaignData(
            campaign,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(entry))))
        return 1;

    return 0;
}

HOOK(0x00516AA0, ProfilesSavegames_ReadProfileEntry, "auto");
