#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, ProfilesSavegames_CombinePath, 0x00512920, (void* a1, void* a2, void* a3));
CSF_FUNC(void*, __fastcall, ProfilesSavegames_GetSavePrefix, 0x00512C60, (int a1, int a2, void* a3));
CSF_FUNC(void*, __fastcall, CSaveGameManager_GetExcludedSaveName, 0x00512F60, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(std::int32_t, __cdecl, ParseThumbnailIndex, 0x007AFD89, (const char* text));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

using GetCurrentDirectoryWProc = std::uint32_t (__stdcall*)(std::uint32_t, std::uint16_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const std::uint16_t*);
using FindFirstFileAProc = void* (__stdcall*)(const char*, void*);
using FindNextFileAProc = int (__stdcall*)(void*, void*);
using FindCloseProc = int (__stdcall*)(void*);

CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);
CSF_GLOBAL(FindFirstFileAProc, g_FindFirstFileA, 0x0082D0DC);
CSF_GLOBAL(FindNextFileAProc, g_FindNextFileA, 0x0082D184);
CSF_GLOBAL(FindCloseProc, g_FindClose, 0x0082D18C);

static char* __fastcall CSaveGameManager_GetNextSaveFilename(
    CSaveGameManager* self, void* /*edx*/, C_0082d410* output)
{
    std::uint16_t originalDirectory[1024];
    char excludedName[256];
    alignas(8) std::uint8_t findData[0x140];
    char normalizedName[256];
    char filename[256];

    alignas(8) std::uint8_t searchPathBytes[0x1C];
    alignas(8) std::uint8_t excludedBytes[0x1C];
    alignas(8) std::uint8_t prefixBytes[0x24];
    auto& searchPath = *reinterpret_cast<C_0082d410*>(searchPathBytes);
    auto& excluded = *reinterpret_cast<C_0082d410*>(excludedBytes);
    auto& prefix = *reinterpret_cast<C_0082d410*>(prefixBytes);

    const char originalFileOption = GetOptionsByteAt2();
    if (originalFileOption)
        SceneLoader_SetFileOption(0);

    unsigned int largestIndex = 0;
    CSF_AT(std::uint32_t, &excluded, 0x18) = 15;
    CSF_AT(std::uint32_t, &excluded, 0x14) = 0;
    reinterpret_cast<char*>(&excluded.field_0004)[0] = 0;
    CSaveGameManager_GetExcludedSaveName(self, nullptr, &excluded);

    char* excludedText = CSF_AT(std::uint32_t, &excluded, 0x18) < 0x10
        ? reinterpret_cast<char*>(&excluded.field_0004)
        : reinterpret_cast<char*>(excluded.field_0004);
    NormalizeResourceName(excludedText, excludedName, 256);

    CSF_AT(std::uint32_t, &prefix, 0x18) = 15;
    CSF_AT(std::uint32_t, &prefix, 0x14) = 0;
    reinterpret_cast<char*>(&prefix.field_0004)[0] = 0;
    g_GetCurrentDirectoryW(0x400, originalDirectory);

    const char originalOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const std::uint16_t* saveDirectory = self->field_0074 < 8
        ? reinterpret_cast<const std::uint16_t*>(&self->field_0060)
        : reinterpret_cast<const std::uint16_t*>(self->field_0060);
    g_SetCurrentDirectoryW(saveDirectory);

    ProfilesSavegames_GetSavePrefix(reinterpret_cast<int>(self), 0, &prefix);
    auto* combinedPath = reinterpret_cast<C_0082d410*>(
        ProfilesSavegames_CombinePath(
            &searchPath, &prefix,
            const_cast<char*>(CSF_STR(0x008408B8, "*.sav"))));
    const char* pattern = CSF_AT(std::uint32_t, combinedPath, 0x18) < 0x10
        ? reinterpret_cast<const char*>(&combinedPath->field_0004)
        : reinterpret_cast<const char*>(combinedPath->field_0004);

    void* searchHandle = g_FindFirstFileA(pattern, findData);
    if (CSF_AT(std::uint32_t, &searchPath, 0x18) >= 0x10 &&
        searchPath.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(searchPath.field_0004));

    if (searchHandle != reinterpret_cast<void*>(-1))
    {
        const FindNextFileAProc findNext = g_FindNextFileA;
        do
        {
            NormalizeResourceName(
                reinterpret_cast<char*>(findData + 0x2C),
                normalizedName, 256);
            if (__stricmp(normalizedName, excludedName))
            {
                const unsigned int index =
                    static_cast<unsigned int>(ParseThumbnailIndex(normalizedName));
                if (index > largestIndex)
                    largestIndex = index;
            }
        } while (findNext(searchHandle, findData));
        g_FindClose(searchHandle);
    }

    const char* prefixText = CSF_AT(std::uint32_t, &prefix, 0x18) < 0x10
        ? reinterpret_cast<const char*>(&prefix.field_0004)
        : reinterpret_cast<const char*>(prefix.field_0004);
    _sprintf(filename, CSF_STR(0x008408AC, "%s%d.sav"),
             prefixText, largestIndex + 1);
    AssignStringRange_429B60(
        output, nullptr, filename,
        static_cast<unsigned int>(std::strlen(filename)));

    g_SetCurrentDirectoryW(originalDirectory);
    StoreOptionsByte104(originalOption104);
    SceneLoader_SetFileOption(originalFileOption);

    char* result = reinterpret_cast<char*>(
        CSF_AT(std::uint32_t, &prefix, 0x18));
    if (CSF_AT(std::uint32_t, &prefix, 0x18) >= 0x10)
    {
        result = reinterpret_cast<char*>(prefix.field_0004);
        if (prefix.field_0004)
            result = reinterpret_cast<char*>(
                Mem_FreeDebug(reinterpret_cast<void*>(prefix.field_0004)));
    }

    CSF_AT(std::uint32_t, &prefix, 0x18) = 15;
    CSF_AT(std::uint32_t, &prefix, 0x14) = 0;
    reinterpret_cast<char*>(&prefix.field_0004)[0] = 0;

    if (CSF_AT(std::uint32_t, &excluded, 0x18) >= 0x10)
    {
        result = reinterpret_cast<char*>(excluded.field_0004);
        if (excluded.field_0004)
            return reinterpret_cast<char*>(
                Mem_FreeDebug(reinterpret_cast<void*>(excluded.field_0004)));
    }
    return result;
}

HOOK(0x00513250, CSaveGameManager_GetNextSaveFilename, "auto");
