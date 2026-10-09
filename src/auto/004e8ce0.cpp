#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(void, __cdecl, SceneLoader_RestorePreviousLog, 0x006981B0, ());
CSF_FUNC(void, __cdecl, SceneLoader_SelectArchive, 0x00698960, (char* String1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(const char*, g_pszKey_MUNDOVIS, 0x008A9A14);
CSF_GLOBAL(const char*, g_pszKey_INICIO_COMMANDO, 0x008A9BCC);
CSF_GLOBAL(const char*, g_pszKey_INICIO_SNIPER, 0x008A9BD0);
CSF_GLOBAL(const char*, g_pszKey_INICIO_SPY, 0x008A9BD4);

// Opens a mission definition file and reads which characters (commando / sniper / spy)
// have a start position defined in its .MUNDOVIS section.
static char __stdcall SceneMissionSetup_ReadAvailableStartCharacters(const char* missionPath,
                                                                     bool* hasCommandoStart,
                                                                     bool* hasSniperStart,
                                                                     bool* hasSpyStart)
{
    uint8_t missionFile[0x3C];
    char archivePath[0x200];
    char loaded;

    MenuExtras_InitializeFile(missionFile, nullptr);

    uint8_t* globalConfig = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(uint8_t, globalConfig, 0xA5C) != 0)
    {
        // Data is packed: redirect the loader to the matching .pak archive while reading.
        std::strcpy(archivePath, missionPath);
        Path_ReplaceExtension(archivePath, 0x200, const_cast<char*>(CSF_STR(0x00830FEC, ".pak")));
        SceneLoader_SelectArchive(archivePath);
        loaded = MenuExtras_LoadFile(missionFile, nullptr, missionPath);
        SceneLoader_RestorePreviousLog();
    }
    else
    {
        loaded = MenuExtras_LoadFile(missionFile, nullptr, missionPath);
    }

    if (loaded)
    {
        uint8_t* worldSection = LookupNamedElement(CSF_AT(uint8_t*, missionFile, 0), nullptr, g_pszKey_MUNDOVIS);
        if (worldSection != nullptr)
        {
            *hasCommandoStart = ReadIntegerProperty(worldSection, nullptr, const_cast<char*>(g_pszKey_INICIO_COMMANDO)) != 0;
            *hasSniperStart = ReadIntegerProperty(worldSection, nullptr, const_cast<char*>(g_pszKey_INICIO_SNIPER)) != 0;
            *hasSpyStart = ReadIntegerProperty(worldSection, nullptr, const_cast<char*>(g_pszKey_INICIO_SPY)) != 0;
            MenuExtras_CloseFile(missionFile, nullptr);
            return 1;
        }
    }

    MenuExtras_CloseFile(missionFile, nullptr);
    return 0;
}

HOOK(0x004E8CE0, SceneMissionSetup_ReadAvailableStartCharacters, "auto");
