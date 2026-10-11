#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, ProfilesSavegames_WriteProfileSettings, 0x00511DB0, (void* a1, int a2));
CSF_FUNC(int, __stdcall, ProfilesSavegames_WriteProfileIdentity, 0x00515A50, (void* a1, int a2));
CSF_FUNC(int, __stdcall, ProfilesSavegames_WriteProfileDetails, 0x00515AF0, (void* a1, int a2));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginRecord, 0x006951B0, (void* self, void* edx, char a2, char a3, int a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndRecord, 0x006951D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup, 0x00695260, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));

CSF_GLOBAL(const char*, g_pszKey_VERSION_profiles_savegames, 0x00899BC0);
CSF_GLOBAL(const char*, g_pszKey_PERFILES, 0x00899BC4);
CSF_GLOBAL(const char*, g_pszKey_KEY, 0x00899BCC);
CSF_GLOBAL(const char*, g_pszKey_CURRENT, 0x00899BD0);
CSF_GLOBAL(const char*, g_pszKey_COMMANDO_DIF, 0x00899BE8);

static int __stdcall ProfilesSavegames_WriteProfiles(S_00694cd0* log, int profilesAddress)
{
    CoreZlibLog_BeginRecord(log, nullptr, 0, 0, 0);
    CoreZlibLog_WriteInteger(
        log, nullptr, 13, const_cast<char*>(g_pszKey_VERSION_profiles_savegames));
    CoreZlibLog_BeginGroup(
        log, nullptr, const_cast<char*>(g_pszKey_PERFILES), 1, 0);
    GetGlobalValue_8B056C();

    void* profiles = reinterpret_cast<void*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(profilesAddress)));

    uint32_t remaining = CSF_AT(uint32_t, profiles, 0x04);
    if (remaining != 0) {
        const uint32_t byteCount = CSF_AT(uint32_t, profiles, 0x08) - remaining;
        remaining = static_cast<uint32_t>(static_cast<int32_t>(byteCount) / 80);
    }

    uint32_t offset = 0;
    while (remaining != 0) {
        const uint32_t profileAddress = CSF_AT(uint32_t, profiles, 0x04) + offset;
        void* profile = reinterpret_cast<void*>(static_cast<uintptr_t>(profileAddress));

        CoreZlibLog_BeginRecord(log, nullptr, 0, 0, 0);
        ProfilesSavegames_WriteProfileIdentity(log, static_cast<int>(profileAddress));
        CoreZlibLog_WriteInteger(
            log, nullptr, CSF_AT(int, profile, 0x48),
            const_cast<char*>(g_pszKey_KEY));
        CoreZlibLog_WriteInteger(
            log, nullptr, CSF_AT(uint8_t, profile, 0x4C) != 0,
            const_cast<char*>(g_pszKey_CURRENT));
        ProfilesSavegames_WriteProfileSettings(
            log, static_cast<int>(profileAddress + 28));
        CoreZlibLog_WriteInteger(
            log, nullptr, CSF_AT(uint8_t, profile, 0x44) != 0,
            const_cast<char*>(g_pszKey_COMMANDO_DIF));
        ProfilesSavegames_WriteProfileDetails(log, static_cast<int>(profileAddress));
        CoreZlibLog_EndRecord(log, nullptr);

        offset += 80;
        --remaining;
    }

    CoreZlibLog_EndGroup(log, nullptr);
    return static_cast<int>(CoreZlibLog_EndRecord(log, nullptr));
}

HOOK(0x00515E80, ProfilesSavegames_WriteProfiles, "auto");
