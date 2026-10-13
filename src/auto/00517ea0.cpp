#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using GetCurrentDirectoryWFunction = std::uint32_t (__stdcall*)(std::uint32_t, wchar_t*);
using SetCurrentDirectoryWFunction = int (__stdcall*)(const wchar_t*);

CSF_GLOBAL(GetCurrentDirectoryWFunction, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWFunction, g_SetCurrentDirectoryW, 0x0082D190);

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __stdcall, NetProfile_ReadEntry, 0x00518530, (int a1, void* a2));
CSF_FUNC(int, __fastcall, NetProfile_ValidateEntry, 0x00518860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuAccountEdit_ResetInnerObject, 0x00518890, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));

static char __fastcall NetProfile_LoadProfiles(void* self, void* /*edx*/)
{
    alignas(8) std::uint8_t directoryBytes[0x808];
    auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);
    g_GetCurrentDirectoryW(0x400u, previousDirectory);

    const char previousOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* profileDirectory;
    if (CSF_AT(std::uint32_t, self, 0x1740) < 8u)
        profileDirectory = reinterpret_cast<const wchar_t*>(
            static_cast<std::uint8_t*>(self) + 0x172C);
    else
        profileDirectory = CSF_AT(const wchar_t*, self, 0x172C);

    const auto setDirectory = g_SetCurrentDirectoryW;
    if (!setDirectory(profileDirectory))
    {
        setDirectory(previousDirectory);
        StoreOptionsByte104(previousOption104);
        return 0;
    }

    const char previousFileOption = GetOptionsByteAt2();
    if (previousFileOption)
        CoreFiles_SetFileOption(0);

    CSF_AT(std::uint32_t, self, 0x1704) = 0;
    const char previousFlag1 = GetOptionsByte1();
    Options_SetFlag1(0);

    alignas(8) std::uint8_t filenameBytes[0x24];
    auto& filename = *reinterpret_cast<C_0082d410*>(filenameBytes);
    auto* entry = static_cast<std::uint8_t*>(self) + 12;

    for (int index = 0; index < 10; ++index)
    {
        if (CSF_AT(std::uint32_t, self, 0x1704))
            break;

        CMenuAccountEdit_ResetInnerObject(entry, nullptr);

        CSF_AT(std::uint32_t, filenameBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, filenameBytes, 0x14) = 0;
        CSF_AT(char, filenameBytes, 0x4) = 0;

        AssignStringRange_429B60(
            &filename, nullptr, const_cast<char*>(CSF_STR(0x00840B8C, "PERF")), 4);
        StringAppendFill(&filename, nullptr, 1, static_cast<char>(index / 10 + 48));
        StringAppendFill(&filename, nullptr, 1, static_cast<char>(index % 10 + 48));
        Profiles_AppendStringRange(
            &filename, 0, const_cast<char*>(CSF_STR(0x00840B84, ".PLB")), 4);

        const char* filenameText = reinterpret_cast<const char*>(filename.field_0004);
        if (CSF_AT(std::uint32_t, filenameBytes, 0x18) < 0x10)
            filenameText = reinterpret_cast<const char*>(&filename.field_0004);

        void* file = CoreFiles_OpenSceneFile(
            filenameText, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);

        if (file)
        {
            if (NetProfile_ReadEntry(static_cast<int>(reinterpret_cast<std::uintptr_t>(file)), entry))
                CSF_AT(std::uint32_t, self, 0x1704) = 3;
            else if (static_cast<std::uint8_t>(NetProfile_ValidateEntry(entry, nullptr)) == 0)
                CSF_AT(std::uint32_t, self, 0x1704) = 3;

            CloseSceneFile(file);
        }

        if (CSF_AT(std::uint32_t, filenameBytes, 0x18) >= 0x10)
        {
            if (filename.field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(filename.field_0004));
        }

        entry += 0x24C;
        CSF_AT(std::uint32_t, filenameBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, filenameBytes, 0x14) = 0;
        CSF_AT(char, filenameBytes, 0x4) = 0;
    }

    Options_SetFlag1(previousFlag1);
    g_SetCurrentDirectoryW(previousDirectory);
    StoreOptionsByte104(previousOption104);
    CSF_AT(char, self, 0x1708) = 0;
    CoreFiles_SetFileOption(previousFileOption);
    return 0;
}

HOOK(0x00517EA0, NetProfile_LoadProfiles, "auto");
