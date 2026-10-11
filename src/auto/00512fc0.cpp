#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, CSaveGameManager_PrepareSaveName, 0x00512F60, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));

using GetCurrentDirectoryWFunction =
    std::uint32_t (__stdcall*)(std::uint32_t size, wchar_t* buffer);
using SetCurrentDirectoryWFunction =
    int (__stdcall*)(const wchar_t* directory);

CSF_GLOBAL(GetCurrentDirectoryWFunction, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWFunction, g_SetCurrentDirectoryW, 0x0082D190);

static int __fastcall CSaveGameManager_ValidateSaveName(
    CSaveGameManager* self, void* /*edx*/, C_0082d410* saveName)
{
    alignas(8) std::uint8_t directoryBytes[0x800];
    alignas(8) std::uint8_t emptyStringBytes[0x20];
    C_0082d410& emptyString =
        *reinterpret_cast<C_0082d410*>(emptyStringBytes);

    CSaveGameManager_PrepareSaveName(self, nullptr, saveName);

    const unsigned char previousFileOption =
        static_cast<unsigned char>(GetOptionsByteAt2());
    if (previousFileOption)
        SceneLoader_SetFileOption(0);

    auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);
    g_GetCurrentDirectoryW(0x400u, previousDirectory);

    const char previousOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* saveDirectory =
        self->field_0074 < 8
            ? reinterpret_cast<const wchar_t*>(&self->field_0060)
            : reinterpret_cast<const wchar_t*>(self->field_0060);

    bool available = false;
    if (g_SetCurrentDirectoryW(saveDirectory))
    {
        const char* name =
            CSF_AT(std::uint32_t, saveName, 0x18) < 0x10
                ? reinterpret_cast<const char*>(&saveName->field_0004)
                : reinterpret_cast<const char*>(saveName->field_0004);
        available = MenuManager_CheckPageAvailability(name, 0);
    }

    const C_0082d410* source = saveName;
    if (!available)
    {
        CSF_AT(std::uint32_t, &emptyString, 0x18) = 15;
        CSF_AT(std::uint32_t, &emptyString, 0x14) = 0;
        reinterpret_cast<unsigned char*>(&emptyString.field_0004)[0] = 0;
        AssignStringRange_429B60(
            &emptyString, nullptr,
            const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);
        source = &emptyString;
    }

    AssignStringSubstring(saveName, nullptr, source, 0, -1);

    if (!available &&
        CSF_AT(std::uint32_t, &emptyString, 0x18) >= 0x10 &&
        emptyString.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(emptyString.field_0004));
    }

    g_SetCurrentDirectoryW(previousDirectory);
    StoreOptionsByte104(previousOption104);
    return SceneLoader_SetFileOption(static_cast<char>(previousFileOption));
}

HOOK(0x00512FC0, CSaveGameManager_ValidateSaveName, "auto");
