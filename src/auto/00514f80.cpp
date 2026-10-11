#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(uint32_t*, __fastcall, ProfilesSavegames_AppendCharacter, 0x00697020, (void* self, void* edx, int16_t a2));
CSF_FUNC(void*, __fastcall, ProfilesSavegames_AppendNarrowText, 0x00697150, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(unsigned int, __cdecl, ProfilesSavegames_GetUserDataPath, 0x0069D580, (wchar_t* path));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

using CreateDirectoryWProc = int (__stdcall*)(const wchar_t*, void*);
CSF_GLOBAL(CreateDirectoryWProc, g_CreateDirectoryW, 0x0082D218);

static int __fastcall CProfileManager_InitializeUserDataPaths(
    CProfileManager* self, void* /*edx*/)
{
    int result = GetGlobalValue_75FDF0();
    if (!CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0xCC))
        return result;

    alignas(8) std::uint8_t pathBytes[0x204];
    if (!ProfilesSavegames_GetUserDataPath(
            reinterpret_cast<wchar_t*>(pathBytes)))
        return 0;

    auto* objectBytes = reinterpret_cast<std::uint8_t*>(self);
    auto* profileIndexPath = reinterpret_cast<S_004f21d0*>(objectBytes + 0x68);
    auto* profilesPath = reinterpret_cast<S_004f21d0*>(objectBytes + 0x4C);
    auto* gamesPath = reinterpret_cast<S_004f21d0*>(objectBytes + 0x30);
    auto* dataPath = reinterpret_cast<S_004f21d0*>(objectBytes + 0x84);

    auto* assignedPath = MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(profileIndexPath), nullptr,
        reinterpret_cast<std::int16_t*>(pathBytes));
    AssignStringRange_488830(profilesPath, nullptr, assignedPath, 0, 0xFFFFFFFF);
    AssignStringRange_488830(gamesPath, nullptr, profilesPath, 0, 0xFFFFFFFF);
    AssignStringRange_488830(dataPath, nullptr, gamesPath, 0, 0xFFFFFFFF);

    ProfilesSavegames_AppendCharacter(gamesPath, nullptr, 92);
    ProfilesSavegames_AppendCharacter(profilesPath, nullptr, 92);
    ProfilesSavegames_AppendCharacter(profileIndexPath, nullptr, 92);
    ProfilesSavegames_AppendCharacter(dataPath, nullptr, 92);

    ProfilesSavegames_AppendNarrowText(
        dataPath, nullptr,
        reinterpret_cast<uint8_t*>(const_cast<char*>(
            CSF_STR(0x0083FFF4, "CSF Data"))));
    ProfilesSavegames_AppendNarrowText(
        gamesPath, nullptr,
        reinterpret_cast<uint8_t*>(const_cast<char*>(
            CSF_STR(0x00840890, "CSF Data\\Games\\"))));
    ProfilesSavegames_AppendNarrowText(
        profilesPath, nullptr,
        reinterpret_cast<uint8_t*>(const_cast<char*>(
            CSF_STR(0x008409DC, "CSF Data\\Profiles"))));
    ProfilesSavegames_AppendNarrowText(
        profileIndexPath, nullptr,
        reinterpret_cast<uint8_t*>(const_cast<char*>(
            CSF_STR(0x008409C0, "CSF Data\\Profiles\\Profiles"))));

    const wchar_t* directory;
    if (dataPath->field_0018 < 8)
        directory = reinterpret_cast<const wchar_t*>(&dataPath->field_0004);
    else
        std::memcpy(&directory, &dataPath->field_0004, sizeof(directory));

    return g_CreateDirectoryW(directory, nullptr);
}

HOOK(0x00514F80, CProfileManager_InitializeUserDataPaths, "auto");
