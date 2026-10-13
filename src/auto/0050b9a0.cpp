#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(unsigned int, __cdecl, ProfilesSavegames_GetBaseDirectory, 0x0069D580, (wchar_t* path));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* edx, uint8_t* a2));

using CreateDirectoryWFunction = int (__stdcall*)(const wchar_t*, void*);
CSF_GLOBAL(CreateDirectoryWFunction, g_CreateDirectoryW, 0x0082D218);

static int __fastcall COptionsFile_InitializeOptionsPaths(COptionsFile* self, void* /*edx*/)
{
    int result = GetGlobalValue_75FDF0();
    void* globalState = reinterpret_cast<void*>(static_cast<std::uintptr_t>(result));

    if (!CSF_AT(std::uint8_t, globalState, 0xCC))
        return result;

    alignas(8) std::uint8_t pathBytes[0x200];
    result = static_cast<int>(ProfilesSavegames_GetBaseDirectory(
        reinterpret_cast<wchar_t*>(pathBytes)));
    if (!result)
        return result;

    auto* self8 = reinterpret_cast<std::uint8_t*>(self);
    void* nameObj = self8 + 0x18;
    auto* pathObj = self8 + 0x34;

    void* assignedText = MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(nameObj), nullptr,
        reinterpret_cast<std::int16_t*>(pathBytes));

    AssignStringRange_488830(pathObj, nullptr, assignedText, 0, 0xFFFFFFFF);
    SaveSlots_ApplyIndex(pathObj, nullptr, 92);
    AppendNarrowToWideString(
        pathObj, nullptr,
        reinterpret_cast<std::uint8_t*>(
            const_cast<char*>(CSF_STR(0x0083FFF4, "CSF Data"))));

    SaveSlots_ApplyIndex(nameObj, nullptr, 92);
    AppendNarrowToWideString(
        nameObj, nullptr,
        reinterpret_cast<std::uint8_t*>(
            const_cast<char*>(CSF_STR(0x0083FFE0, "CSF Data\\Options"))));

    if (CSF_AT(std::uint32_t, pathObj, 0x18) < 8)
        return g_CreateDirectoryW(
            reinterpret_cast<const wchar_t*>(pathObj + 4), nullptr);
    return g_CreateDirectoryW(CSF_AT(const wchar_t*, pathObj, 0x04), nullptr);
}

HOOK(0x0050B9A0, COptionsFile_InitializeOptionsPaths, "auto");
