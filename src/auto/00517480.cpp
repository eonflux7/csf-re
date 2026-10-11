#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, ProfilesSavegames_InitializeProfile, 0x005166E0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, ProfilesSavegames_ReadVersion12Profile, 0x00516AA0, (void* self, void* edx, void* a1, void* a2, int arg8));
CSF_FUNC(bool, __fastcall, ProfilesSavegames_ReadVersion13Profile, 0x00516B90, (void* self, void* edx, void* a1, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, ProfilesSavegames_AddProfile, 0x00517230, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_VERSION_profiles_savegames, 0x00899BC0);
CSF_GLOBAL(const char*, g_pszKey_PERFILES, 0x00899BC4);

// The packet also identifies the child-count address as a vtable reference.
[[maybe_unused]] static const auto g_ProfileNodeVtable = CSF_VTBL(0x00692B80);

static char __fastcall C_00840af8_LoadProfiles(
    C_00840af8* self, void* /*edx*/,
    S_00692b80** root, S_005170e0* destination)
{
    auto* node = reinterpret_cast<std::uint8_t*>(*root);
    const int version = ReadIntegerAttribute(
        node, nullptr, const_cast<char*>(g_pszKey_VERSION_profiles_savegames));
    if (version > 13)
        return 0;

    auto* profiles = FindProperty(
        node, nullptr, const_cast<char*>(g_pszKey_PERFILES));
    if (!profiles)
        return 0;

    char success = 1;
    const std::uint32_t childCount = MenuExtras_GetChildCount(profiles, nullptr);
    alignas(8) std::uint8_t profileBytes[0x60] = {};

    for (unsigned int index = 0; index < childCount; ++index)
    {
        if (!success)
            return success;

        CSF_AT(std::uint32_t, profileBytes, 0x18) = 7;
        CSF_AT(int, profileBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, profileBytes, 0x04) = 0;
        CSF_AT(int, profileBytes, 0x2C) = -1;
        CSF_AT(int, profileBytes, 0x28) = -1;
        CSF_AT(int, profileBytes, 0x24) = -1;
        CSF_AT(int, profileBytes, 0x20) = -1;
        CSF_AT(int, profileBytes, 0x1C) = -1;
        CSF_AT(int, profileBytes, 0x30) = 0;
        CSF_AT(void*, profileBytes, 0x38) = nullptr;
        CSF_AT(int, profileBytes, 0x3C) = 0;
        CSF_AT(int, profileBytes, 0x40) = 0;

        ProfilesSavegames_InitializeProfile(profileBytes, nullptr);
        auto* component = reinterpret_cast<void*>(
            GetComponent(profiles, nullptr, index));

        bool loaded = false;
        if (version == 12)
        {
            loaded = ProfilesSavegames_ReadVersion12Profile(
                self, nullptr, component, profileBytes, 12);
        }
        else if (version == 13)
        {
            loaded = ProfilesSavegames_ReadVersion13Profile(
                self, nullptr, component,
                static_cast<int>(reinterpret_cast<std::intptr_t>(profileBytes)), 13);
        }

        if (!loaded || CSF_AT(int, profileBytes, 0x1C) == -1)
        {
            success = 0;
        }
        else
        {
            success = 1;
            ProfilesSavegames_AddProfile(
                destination, nullptr,
                static_cast<int>(reinterpret_cast<std::intptr_t>(profileBytes)));
        }

        if (CSF_AT(void*, profileBytes, 0x38))
            Mem_FreeDebug(CSF_AT(void*, profileBytes, 0x38));

        CSF_AT(void*, profileBytes, 0x38) = nullptr;
        CSF_AT(int, profileBytes, 0x3C) = 0;
        CSF_AT(int, profileBytes, 0x40) = 0;

        if (CSF_AT(std::uint32_t, profileBytes, 0x18) >= 8)
        {
            if (CSF_AT(void*, profileBytes, 0x04))
                Mem_FreeDebug(CSF_AT(void*, profileBytes, 0x04));
        }

        CSF_AT(std::uint32_t, profileBytes, 0x18) = 7;
        CSF_AT(int, profileBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, profileBytes, 0x04) = 0;
    }

    return success;
}

HOOK(0x00517480, C_00840af8_LoadProfiles, "auto");
