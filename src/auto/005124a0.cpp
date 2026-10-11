#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));

using GetCurrentDirectoryWProc = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const wchar_t*);
using SetFileAttributesAProc = int (__stdcall*)(const char*, uint32_t);
using DeleteFileAProc = int (__stdcall*)(const char*);

CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);
CSF_GLOBAL(SetFileAttributesAProc, g_SetFileAttributesA, 0x0082D118);
CSF_GLOBAL(DeleteFileAProc, g_DeleteFileA, 0x0082D210);

static char __fastcall CSaveGameManager_DeleteMatchingSaveGame(
    CSaveGameManager* self, void* /*edx*/, uint32_t* save)
{
    alignas(8) uint8_t directoryBytes[0x810];
    auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);

    const char previousFileOption = GetOptionsByteAt2();
    if (previousFileOption)
        SceneLoader_SetFileOption(0);

    self->vSlot18(0);
    g_GetCurrentDirectoryW(0x400, previousDirectory);

    const char previousOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* saveDirectory;
    if (self->field_0074 < 8)
        saveDirectory = reinterpret_cast<const wchar_t*>(&self->field_0060);
    else
        saveDirectory = reinterpret_cast<const wchar_t*>(self->field_0060);
    g_SetCurrentDirectoryW(saveDirectory);

    const uint32_t entriesBegin = self->field_000c;
    const uint32_t entryCount = entriesBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0010 - entriesBegin) / 200)
        : 0;

    char deleted = 0;
    uint32_t entryOffset = 0;
    for (uint32_t index = 0; index < entryCount; ++index, entryOffset += 200)
    {
        auto* entryName = reinterpret_cast<uint8_t*>(
            self->field_000c + entryOffset) + 0x50;

        const uint32_t nameLength = CSF_AT(uint32_t, save, 0x14);
        const char* nameStorage = reinterpret_cast<const char*>(save) + 4;
        const char* name = CSF_AT(uint32_t, save, 0x18) < 0x10
            ? nameStorage
            : reinterpret_cast<const char*>(CSF_AT(uint32_t, save, 4));

        if (CompareStringSubstring(
                entryName, nullptr, 0,
                CSF_AT(uint32_t, entryName, 0x14), name, nameLength) != 0)
            continue;

        if (CSF_AT(uint32_t, save, 0x18) >= 0x10)
            nameStorage = reinterpret_cast<const char*>(
                CSF_AT(uint32_t, save, 4));

        if (!MenuManager_CheckPageAvailability(nameStorage, 0))
            continue;

        self->field_0018 = 1;

        const char* inlineName = reinterpret_cast<const char*>(save) + 4;
        const char* attributesName = CSF_AT(uint32_t, save, 0x18) < 0x10
            ? inlineName
            : reinterpret_cast<const char*>(CSF_AT(uint32_t, save, 4));
        g_SetFileAttributesA(attributesName, 0x20);

        const char* deletionName = inlineName;
        if (CSF_AT(uint32_t, save, 0x18) >= 0x10)
            deletionName = reinterpret_cast<const char*>(
                CSF_AT(uint32_t, save, 4));
        g_DeleteFileA(deletionName);

        deleted = 1;
        break;
    }

    g_SetCurrentDirectoryW(previousDirectory);
    StoreOptionsByte104(previousOption104);
    SceneLoader_SetFileOption(previousFileOption);
    return deleted;
}

HOOK(0x005124A0, CSaveGameManager_DeleteMatchingSaveGame, "auto");
