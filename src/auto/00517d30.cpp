#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_ConstructFromText, 0x00434150, (C_0082d410* self, void* , uint32_t* a2));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(bool, __cdecl, CoreFiles_DeleteFile, 0x006976C0, (const char* path));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));

using GetCurrentDirectoryWFn = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using SetCurrentDirectoryWFn = int (__stdcall*)(const wchar_t*);

CSF_GLOBAL(GetCurrentDirectoryWFn, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWFn, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall NetProfile_DeleteProfile(void* self, void* /*edx*/, int profileIndex)
{
    alignas(8) uint8_t directoryBytes[0x800];
    auto* originalDirectory = reinterpret_cast<wchar_t*>(directoryBytes);

    g_GetCurrentDirectoryW(0x400u, originalDirectory);
    const char savedOptionsByte104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const auto setCurrentDirectory = g_SetCurrentDirectoryW;
    if (profileIndex < 0 || profileIndex >= 10)
    {
        setCurrentDirectory(originalDirectory);
        StoreOptionsByte104(savedOptionsByte104);
        CSF_AT(uint32_t, self, 0x1704) = 12;
        return 0;
    }

    const wchar_t* profileDirectory;
    if (CSF_AT(uint32_t, self, 0x1740) < 8u)
        profileDirectory = reinterpret_cast<const wchar_t*>(
            reinterpret_cast<uint8_t*>(self) + 0x172C);
    else
        profileDirectory = CSF_AT(const wchar_t*, self, 0x172C);

    if (!setCurrentDirectory(profileDirectory))
    {
        setCurrentDirectory(originalDirectory);
        StoreOptionsByte104(savedOptionsByte104);
        CSF_AT(uint32_t, self, 0x1704) = 12;
        return 0;
    }

    CSF_AT(uint32_t, self, 0x1704) = 0;
    const char savedFileOption = GetOptionsByteAt2();
    if (savedFileOption)
        CoreFiles_SetFileOption(0);

    alignas(8) uint8_t filenameBytes[0x24];
    auto& filename = *reinterpret_cast<C_0082d410*>(filenameBytes);

    C_0082d410_ConstructFromText(
        &filename, nullptr,
        reinterpret_cast<uint32_t*>(const_cast<char*>(CSF_STR(0x00840B8C, "PERF"))));
    StringAppendFill(&filename, nullptr, 1, static_cast<char>(profileIndex / 10 + 48));
    StringAppendFill(&filename, nullptr, 1, static_cast<char>(profileIndex % 10 + 48));
    Profiles_AppendStringRange(
        &filename, 0, const_cast<char*>(CSF_STR(0x00840B84, ".PLB")), 4);

    const char* path = reinterpret_cast<const char*>(filename.field_0004);
    if (CSF_AT(uint32_t, &filename, 0x18) < 0x10)
        path = reinterpret_cast<const char*>(&filename.field_0004);

    char deleted;
    if (CoreFiles_DeleteFile(path))
    {
        CSF_AT(uint8_t, self, 0x1708) = 1;
        deleted = 1;
    }
    else
    {
        deleted = 0;
        CSF_AT(uint32_t, self, 0x1704) = 10;
    }

    setCurrentDirectory(originalDirectory);
    StoreOptionsByte104(savedOptionsByte104);
    CoreFiles_SetFileOption(savedFileOption);
    CMenuSaveGame_ResetStringStorage(&filename, nullptr);
    return deleted;
}

HOOK(0x00517D30, NetProfile_DeleteProfile, "auto");
