#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));

using GetCurrentDirectoryWFunction =
    uint32_t (__stdcall*)(uint32_t capacity, wchar_t* buffer);
using SetCurrentDirectoryWFunction =
    int (__stdcall*)(const wchar_t* directory);

CSF_GLOBAL(GetCurrentDirectoryWFunction, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWFunction, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall COptionsFile_ProcessOptionsInDirectory(
    COptionsFile* self, void* /*edx*/)
{
    alignas(8) uint8_t directoryBytes[0x80C];
    wchar_t* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);

    const char previousFileOption = GetOptionsByteAt2();
    if (previousFileOption)
        CoreFiles_SetFileOption(0);

    const char previousOption40 = GetOptionsByte40();
    if (!previousOption40)
        SetOptionsByteAt40(1);

    g_GetCurrentDirectoryW(0x400u, previousDirectory);

    const char previousOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    char result = 0;
    const wchar_t* optionsDirectory;
    if (self->field_0030 < 8)
        optionsDirectory = reinterpret_cast<const wchar_t*>(&self->field_001c);
    else
        optionsDirectory = reinterpret_cast<const wchar_t*>(self->field_001c);

    const SetCurrentDirectoryWFunction setCurrentDirectory = g_SetCurrentDirectoryW;
    if (setCurrentDirectory(optionsDirectory))
        result = self->vSlot2C(const_cast<char*>(CSF_STR(0x0083FFA0, "Options")));

    setCurrentDirectory(previousDirectory);
    StoreOptionsByte104(previousOption104);
    SetOptionsByteAt40(previousOption40);
    CoreFiles_SetFileOption(previousFileOption);
    return result;
}

HOOK(0x0050B710, COptionsFile_ProcessOptionsInDirectory, "auto");
