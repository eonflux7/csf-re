#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, sub_43EB30, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __cdecl, sub_512920, 0x00512920, (void* a1, void* a2, void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

using GetCurrentDirectoryWProc = uint32_t (__stdcall*)(uint32_t, char16_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const char16_t*);
using FindFirstFileAProc = void* (__stdcall*)(const char*, void*);
using FindCloseProc = int (__stdcall*)(void*);

CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);
CSF_GLOBAL(FindFirstFileAProc, g_FindFirstFileA, 0x0082D0DC);
CSF_GLOBAL(FindCloseProc, g_FindClose, 0x0082D18C);

static bool __fastcall CProfileManager_HasNoSaveGames(
    CProfileManager* self, void* /*edx*/, unsigned int profileIndex)
{
    const uint32_t profilesBegin = self->field_0008;
    if (!profilesBegin ||
        profileIndex >= static_cast<uint32_t>(
            static_cast<int32_t>(self->field_000c - profilesBegin) / 80))
    {
        return false;
    }

    alignas(8) uint8_t currentDirectoryBytes[0x800];
    alignas(8) uint8_t findDataBytes[0x140];
    alignas(8) uint8_t searchPatternBytes[0x1C];
    alignas(8) uint8_t directoryBytes[0x1C];
    alignas(8) uint8_t profileNumberBytes[0x8];

    auto* currentDirectory = reinterpret_cast<char16_t*>(currentDirectoryBytes);
    auto& searchPattern = *reinterpret_cast<C_0082d410*>(searchPatternBytes);
    auto& directory = *reinterpret_cast<C_0082d410*>(directoryBytes);
    auto* profileNumber = reinterpret_cast<char*>(profileNumberBytes);

    g_GetCurrentDirectoryW(0x400u, currentDirectory);
    const char previousOption = GetOptionsByte104();
    StoreOptionsByte104(1);

    const char16_t* profilesDirectory;
    if (self->field_0048 < 8)
        profilesDirectory = reinterpret_cast<const char16_t*>(&self->field_0034);
    else
        profilesDirectory = reinterpret_cast<const char16_t*>(self->field_0034);
    g_SetCurrentDirectoryW(profilesDirectory);

    CSF_AT(uint32_t, &directory, 0x18) = 15;
    CSF_AT(uint32_t, &directory, 0x14) = 0;
    reinterpret_cast<char*>(&directory.field_0004)[0] = 0;
    AssignStringRange_429B60(
        &directory, nullptr, const_cast<char*>(CSF_STR(0x00840888, "Games")), 5);

    _sprintf(
        profileNumber,
        CSF_STR(0x0083090C, "%d"),
        CSF_AT(int32_t, reinterpret_cast<void*>(self->field_0008),
               80 * profileIndex + 72));
    sub_43EB30(
        &directory, 0, profileNumber,
        static_cast<unsigned int>(std::strlen(profileNumber)));
    StringAppendFill(&directory, nullptr, 1, 47);

    auto* combinedPattern = reinterpret_cast<C_0082d410*>(
        sub_512920(&searchPattern, &directory,
                   const_cast<char*>(CSF_STR(0x008408B8, "*.sav"))));

    const char* patternText;
    if (CSF_AT(uint32_t, combinedPattern, 0x18) < 0x10u)
        patternText = reinterpret_cast<const char*>(&combinedPattern->field_0004);
    else
        patternText = reinterpret_cast<const char*>(combinedPattern->field_0004);

    void* findHandle = g_FindFirstFileA(patternText, findDataBytes);

    if (CSF_AT(uint32_t, &searchPattern, 0x18) >= 0x10u &&
        searchPattern.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(searchPattern.field_0004));
    }

    const bool foundSave = findHandle != reinterpret_cast<void*>(static_cast<intptr_t>(-1));
    g_FindClose(findHandle);
    g_SetCurrentDirectoryW(currentDirectory);
    StoreOptionsByte104(previousOption);

    const bool hasNoSaveGames = !foundSave;
    if (CSF_AT(uint32_t, &directory, 0x18) >= 0x10u &&
        directory.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(directory.field_0004));
    }
    return hasNoSaveGames;
}

HOOK(0x00514DA0, CProfileManager_HasNoSaveGames, "auto");
