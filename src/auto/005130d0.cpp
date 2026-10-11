#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(unsigned int, __fastcall, SaveGame_DestroyRecord, 0x00500A80, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, S_004f21d0_ResetInnerBuffer, 0x00501170, (S_004f21d0* self, void*));
CSF_FUNC(void*, __fastcall, CSaveGameManager_BuildSaveFileName, 0x00512F60, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));

using GetCurrentDirectoryWFn =
    std::uint32_t (__stdcall*)(std::uint32_t, std::uint16_t*);
using SetCurrentDirectoryWFn =
    int (__stdcall*)(const std::uint16_t*);
using SetFileAttributesAFn =
    int (__stdcall*)(const char*, std::uint32_t);
using DeleteFileAFn = int (__stdcall*)(const char*);

CSF_GLOBAL(GetCurrentDirectoryWFn, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWFn, g_SetCurrentDirectoryW, 0x0082D190);
CSF_GLOBAL(SetFileAttributesAFn, g_SetFileAttributesA, 0x0082D118);
CSF_GLOBAL(DeleteFileAFn, g_DeleteFileA, 0x0082D210);

static char __fastcall CSaveGameManager_SaveCurrentGame(
    CSaveGameManager* self, void* /*edx*/)
{
    alignas(8) std::uint8_t directoryBytes[0x800];
    auto* originalDirectory =
        reinterpret_cast<std::uint16_t*>(directoryBytes);

    // The record includes its embedded objects at +0x1C and +0x50.
    alignas(8) std::uint8_t recordBytes[0xC8];
    S_004f21d0& record =
        *reinterpret_cast<S_004f21d0*>(recordBytes);

    alignas(8) std::uint8_t filenameBytes[0x24];
    C_0082d410& filename =
        *reinterpret_cast<C_0082d410*>(filenameBytes);

    const char originalFileOption = GetOptionsByteAt2();
    if (originalFileOption)
        SceneLoader_SetFileOption(0);

    g_GetCurrentDirectoryW(0x400, originalDirectory);

    const char originalOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const int globalState = GetGlobalValue_8B0568();
    const auto setCurrentDirectory = g_SetCurrentDirectoryW;

    if (CSF_AT(std::uint32_t, reinterpret_cast<void*>(globalState), 0x5C) != 3)
    {
        const auto* saveDirectory = self->field_0074 < 8
            ? reinterpret_cast<const std::uint16_t*>(&self->field_0060)
            : reinterpret_cast<const std::uint16_t*>(self->field_0060);

        if (!setCurrentDirectory(saveDirectory))
        {
            SceneLoader_SetFileOption(originalFileOption);
            setCurrentDirectory(originalDirectory);
            StoreOptionsByte104(originalOption104);
            return 0;
        }
    }

    CSF_AT(std::uint32_t, filenameBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, filenameBytes, 0x14) = 0;
    *reinterpret_cast<char*>(&filename.field_0004) = 0;
    CSaveGameManager_BuildSaveFileName(self, nullptr, &filename);

    const char* attributeFilename =
        reinterpret_cast<const char*>(filename.field_0004);
    if (CSF_AT(std::uint32_t, filenameBytes, 0x18) < 0x10)
        attributeFilename =
            reinterpret_cast<const char*>(&filename.field_0004);
    g_SetFileAttributesA(attributeFilename, 0x20);

    const char* deletionFilename =
        reinterpret_cast<const char*>(filename.field_0004);
    if (CSF_AT(std::uint32_t, filenameBytes, 0x18) < 0x10)
        deletionFilename =
            reinterpret_cast<const char*>(&filename.field_0004);
    g_DeleteFileA(deletionFilename);

    S_004f21d0_ResetInnerBuffer(&record, nullptr);

    ((void (__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, self, 0)[0xC / 4])(
            self, nullptr, &record.field_001c);

    AssignStringRange_488830(
        &record, nullptr,
        reinterpret_cast<std::uint8_t*>(self) + 0x40,
        0, 0xFFFFFFFF);
    AssignStringSubstring(
        recordBytes + 0x50, nullptr, &filename, 0, 0xFFFFFFFF);

    setCurrentDirectory(originalDirectory);
    StoreOptionsByte104(originalOption104);

    const char saved = self->vSlot20(
        reinterpret_cast<std::uint32_t*>(recordBytes));

    SceneLoader_SetFileOption(originalFileOption);
    SaveGame_DestroyRecord(
        reinterpret_cast<std::uint32_t*>(recordBytes), nullptr);

    if (CSF_AT(std::uint32_t, filenameBytes, 0x18) >= 0x10)
    {
        if (filename.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(filename.field_0004));
    }

    return saved;
}

HOOK(0x005130D0, CSaveGameManager_SaveCurrentGame, "auto");
