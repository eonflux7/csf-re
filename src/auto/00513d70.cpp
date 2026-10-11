#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(unsigned int, __fastcall, SaveGame_DestroyRecord, 0x00500A80, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, S_004f21d0_ResetInnerBuffer, 0x00501170, (S_004f21d0* self, void*));
CSF_FUNC(int, __cdecl, SaveGames_CombinePath, 0x00512920, (void* a1, void* a2, void* a3));
CSF_FUNC(void*, __fastcall, CSaveGameManager_GetSearchDirectory, 0x00512C60, (int a1, int a2, void* a3));
CSF_FUNC(int, __cdecl, SaveGames_ClearRecords, 0x00513A30, (int* a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, SaveGames_AppendRecord, 0x00513A80, (void* self, void* edx, int a3));
CSF_FUNC(void, __cdecl, SaveGames_SortRecords, 0x00514140, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));

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

static char* __fastcall CSaveGameManager_RefreshSaveGameList(
    CSaveGameManager* self, void* /*edx*/, char forceRefresh)
{
    alignas(8) std::uint8_t currentDirectoryBytes[0x800];
    alignas(8) std::uint8_t findDataBytes[0x140];
    alignas(8) std::uint8_t recordBytes[0xC8];
    alignas(8) std::uint8_t directoryBytes[0x1C];
    alignas(8) std::uint8_t pathBytes[0x24];

    auto* currentDirectory =
        reinterpret_cast<std::uint16_t*>(currentDirectoryBytes);
    auto& record = *reinterpret_cast<S_004f21d0*>(recordBytes);
    auto& directory = *reinterpret_cast<C_0082d410*>(directoryBytes);
    auto& path = *reinterpret_cast<C_0082d410*>(pathBytes);

    g_GetCurrentDirectoryW(0x400u, currentDirectory);
    const char savedOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    if ((forceRefresh || self->field_0018) &&
        g_SetCurrentDirectoryW(
            self->field_0074 < 8
                ? reinterpret_cast<const std::uint16_t*>(&self->field_0060)
                : reinterpret_cast<const std::uint16_t*>(self->field_0060)))
    {
        const char savedFileOption = GetOptionsByteAt2();
        if (savedFileOption)
            SceneLoader_SetFileOption(0);

        const char savedFlag1 = GetOptionsByte1();
        Options_SetFlag1(0);

        int haveFile = 1;
        SaveGames_ClearRecords(
            reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self) + 8),
            0);

        CSF_AT(std::uint32_t, directoryBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, directoryBytes, 0x14) = 0;
        CSF_AT(std::uint8_t, &directory.field_0004, 0) = 0;
        CSaveGameManager_GetSearchDirectory(
            reinterpret_cast<int>(self), 0, directoryBytes);

        auto* searchPath = reinterpret_cast<C_0082d410*>(
            SaveGames_CombinePath(
                pathBytes, directoryBytes,
                const_cast<char*>(CSF_STR(0x008408B8, "*.sav"))));

        const char* searchPattern =
            CSF_AT(std::uint32_t, searchPath, 0x18) < 0x10u
                ? reinterpret_cast<const char*>(&searchPath->field_0004)
                : reinterpret_cast<const char*>(searchPath->field_0004);

        void* findHandle = g_FindFirstFileA(searchPattern, findDataBytes);

        if (CSF_AT(std::uint32_t, pathBytes, 0x18) >= 0x10u &&
            path.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(path.field_0004));
        }

        if (findHandle != reinterpret_cast<void*>(-1))
        {
            while (haveFile)
            {
                SaveGames_CombinePath(
                    pathBytes, directoryBytes, findDataBytes + 0x2C);
                S_004f21d0_ResetInnerBuffer(&record, nullptr);

                if (self->vSlot1C(
                        reinterpret_cast<int>(recordBytes),
                        reinterpret_cast<int>(pathBytes)))
                {
                    AssignStringSubstring(
                        recordBytes + 0x50, nullptr, pathBytes, 0, -1);
                    SaveGames_AppendRecord(
                        reinterpret_cast<std::uint8_t*>(self) + 8,
                        nullptr, reinterpret_cast<int>(recordBytes));
                }

                SaveGame_DestroyRecord(
                    reinterpret_cast<std::uint32_t*>(recordBytes), nullptr);

                if (CSF_AT(std::uint32_t, pathBytes, 0x18) >= 0x10u &&
                    path.field_0004)
                {
                    Mem_FreeDebug(reinterpret_cast<void*>(path.field_0004));
                }

                CSF_AT(std::uint32_t, pathBytes, 0x18) = 15;
                CSF_AT(std::uint32_t, pathBytes, 0x14) = 0;
                CSF_AT(std::uint8_t, &path.field_0004, 0) = 0;
                haveFile = g_FindNextFileA(findHandle, findDataBytes);
            }
        }

        g_FindClose(findHandle);
        SaveGames_SortRecords(
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 8));
        Options_SetFlag1(savedFlag1);
        SceneLoader_SetFileOption(savedFileOption);
        self->field_0018 = 0;
        g_SetCurrentDirectoryW(currentDirectory);
        StoreOptionsByte104(savedOption104);

        if (CSF_AT(std::uint32_t, directoryBytes, 0x18) >= 0x10u &&
            directory.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(directory.field_0004));
        }
    }
    else
    {
        g_SetCurrentDirectoryW(currentDirectory);
        StoreOptionsByte104(savedOption104);
    }

    return reinterpret_cast<char*>(self) + 8;
}

HOOK(0x00513D70, CSaveGameManager_RefreshSaveGameList, "auto");
