#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(void, __cdecl, SceneLoader_RestorePreviousLog, 0x006981B0, ());
CSF_FUNC(void, __cdecl, SceneLoader_SelectArchive, 0x00698960, (char* String1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);            // empty string ""
CSF_GLOBAL(const char*, g_pszKey_MUNDOVIS, 0x008A9A14);
CSF_GLOBAL(const char*, g_pszKey_VIDEO_OPENING, 0x008A9B34);

// Reads the .MUNDOVIS / .VIDEO_OPENING entry of a mission file into outVideoName.
// Returns 1 when a non-empty video name was found.
static char __stdcall SceneMissionSetup_ReadOpeningVideoName(const char* missionPath, void* outVideoName)
{
    char archivePath[0x200];
    char videoName[0x100];
    uint8_t missionFile[0x3C];

    MenuExtras_InitializeFile(missionFile, nullptr);

    char loaded;
    if (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0()), 0xA5C) != 0)
    {
        // Packed data: select the matching .pak archive while loading
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
        uint8_t* worldSection = LookupNamedElement(CSF_AT(uint8_t*, missionFile, 0x0), nullptr, g_pszKey_MUNDOVIS);
        if (worldSection != nullptr)
        {
            std::memset(videoName, 0, 0x100);
            MenuExtras_ReadText(worldSection, nullptr, const_cast<char*>(g_pszKey_VIDEO_OPENING), videoName, 0x100);

            // strcmp(videoName, "") != 0
            if (static_cast<uint8_t>(videoName[0]) != g_byte_82E65C)
            {
                AssignStringRange_429B60(outVideoName, nullptr, videoName, static_cast<unsigned int>(std::strlen(videoName)));
                MenuExtras_CloseFile(missionFile, nullptr);
                return 1;
            }
        }
    }

    MenuExtras_CloseFile(missionFile, nullptr);
    return 0;
}

HOOK(0x004EE130, SceneMissionSetup_ReadOpeningVideoName, "auto");
