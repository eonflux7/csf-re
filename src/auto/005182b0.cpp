#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(unsigned int, __cdecl, ProfilesSavegames_GetBaseDirectory, 0x0069D580, (wchar_t* path));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

using CreateDirectoryWFunction = int (__stdcall*)(const wchar_t*, void*);
CSF_GLOBAL(CreateDirectoryWFunction, g_CreateDirectoryW, 0x0082D218);

static int __fastcall CNetProfile_CreateDirectories(CNetProfile* self, void* /*edx*/)
{
    int result = GetGlobalValue_75FDF0();
    auto* globalState = reinterpret_cast<void*>(static_cast<std::uintptr_t>(result));
    if (!CSF_AT(std::uint8_t, globalState, 0xCC))
        return result;

    alignas(8) std::uint8_t pathBytes[0x200];
    result = static_cast<int>(
        ProfilesSavegames_GetBaseDirectory(reinterpret_cast<wchar_t*>(pathBytes)));
    if (!result)
        return result;

    auto* profileDirectory = reinterpret_cast<std::uint8_t*>(self) + 0x1728;
    auto* dataDirectory = reinterpret_cast<std::uint8_t*>(self) + 0x1744;

    auto* assignedPath = MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(profileDirectory),
        nullptr,
        reinterpret_cast<std::int16_t*>(pathBytes));
    AssignStringRange_488830(dataDirectory, nullptr, assignedPath, 0, 0xFFFFFFFF);
    SaveSlots_ApplyIndex(dataDirectory, nullptr, 92);
    AppendNarrowToWideString(
        dataDirectory, nullptr,
        reinterpret_cast<std::uint8_t*>(
            const_cast<char*>(CSF_STR(0x0083FFF4, "CSF Data"))));
    SaveSlots_ApplyIndex(profileDirectory, nullptr, 92);
    AppendNarrowToWideString(
        profileDirectory, nullptr,
        reinterpret_cast<std::uint8_t*>(
            const_cast<char*>(CSF_STR(0x00840B94, "CSF Data\\NetProfile"))));
    SaveSlots_ApplyIndex(profileDirectory, nullptr, 92);

    const wchar_t* dataPath;
    if (CSF_AT(std::uint32_t, dataDirectory, 0x18) < 8u)
        dataPath = reinterpret_cast<const wchar_t*>(dataDirectory + 4);
    else
        dataPath = CSF_AT(const wchar_t*, dataDirectory, 0x4);

    const auto createDirectory = g_CreateDirectoryW;
    createDirectory(dataPath, nullptr);

    const wchar_t* profilePath;
    if (CSF_AT(std::uint32_t, profileDirectory, 0x18) < 8u)
        profilePath = reinterpret_cast<const wchar_t*>(profileDirectory + 4);
    else
        profilePath = CSF_AT(const wchar_t*, profileDirectory, 0x4);

    return createDirectory(profilePath, nullptr);
}

HOOK(0x005182B0, CNetProfile_CreateDirectories, "auto");
