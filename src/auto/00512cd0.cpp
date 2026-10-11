#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(uint32_t*, __fastcall, ProfilesSavegames_AppendWideCharacter, 0x00697020, (void* self, void* edx, int16_t a2));
CSF_FUNC(void*, __fastcall, ProfilesSavegames_AppendNarrowText, 0x00697150, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(unsigned int, __cdecl, ProfilesSavegames_GetBaseDirectory, 0x0069D580, (wchar_t* path));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

using CreateDirectoryWFunction = int (__stdcall*)(const wchar_t*, void*);
CSF_GLOBAL(CreateDirectoryWFunction, g_CreateDirectoryW, 0x0082D218);

static int __fastcall CSaveGameManager_InitializeSavePaths(
    CSaveGameManager* self, void* /*edx*/)
{
    int result = GetGlobalValue_75FDF0();
    void* globalState =
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(result));

    if (!CSF_AT(std::uint8_t, globalState, 0xCC))
        return result;

    alignas(8) std::uint8_t pathBytes[0x200];
    result = static_cast<int>(ProfilesSavegames_GetBaseDirectory(
        reinterpret_cast<wchar_t*>(pathBytes)));
    if (!result)
        return result;

    auto* savePath = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x5C);
    auto* assignedPath = MenuProgressInput_AssignText(
        savePath, nullptr, reinterpret_cast<std::int16_t*>(pathBytes));

    void* dataPath = reinterpret_cast<std::uint8_t*>(self) + 0x78;
    AssignStringRange_488830(dataPath, nullptr, assignedPath, 0, 0xFFFFFFFF);
    ProfilesSavegames_AppendWideCharacter(dataPath, nullptr, 92);
    ProfilesSavegames_AppendNarrowText(
        dataPath, nullptr,
        reinterpret_cast<uint8_t*>(
            const_cast<char*>(CSF_STR(0x0083FFF4, "CSF Data"))));

    ProfilesSavegames_AppendWideCharacter(savePath, nullptr, 92);
    ProfilesSavegames_AppendNarrowText(
        savePath, nullptr,
        reinterpret_cast<uint8_t*>(
            const_cast<char*>(CSF_STR(0x00840890, "CSF Data\\Games\\"))));

    const wchar_t* directory;
    if (CSF_AT(std::uint32_t, dataPath, 0x18) < 8)
        directory = reinterpret_cast<const wchar_t*>(
            static_cast<std::uint8_t*>(dataPath) + 4);
    else
        directory = CSF_AT(const wchar_t*, dataPath, 0x04);

    return g_CreateDirectoryW(directory, nullptr);
}

HOOK(0x00512CD0, CSaveGameManager_InitializeSavePaths, "auto");
