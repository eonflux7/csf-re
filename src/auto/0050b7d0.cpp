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

using GetCurrentDirectoryWFn = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using CreateDirectoryWFn = int (__stdcall*)(const wchar_t*, void*);
using SetCurrentDirectoryWFn = int (__stdcall*)(const wchar_t*);

CSF_GLOBAL(GetCurrentDirectoryWFn, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(CreateDirectoryWFn, g_CreateDirectoryW, 0x0082D218);
CSF_GLOBAL(SetCurrentDirectoryWFn, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall COptionsFile_LoadOptions(COptionsFile* self, void* /*edx*/)
{
    alignas(8) uint8_t directoryBytes[0x80C];
    auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);

    const char previousFileOption = GetOptionsByteAt2();
    if (previousFileOption)
        CoreFiles_SetFileOption(0);

    const char previousOption40 = GetOptionsByte40();
    if (!previousOption40)
        SetOptionsByteAt40(1);

    g_GetCurrentDirectoryW(0x400u, previousDirectory);

    const char previousOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* directory =
        self->field_0030 < 8
            ? reinterpret_cast<const wchar_t*>(&self->field_001c)
            : reinterpret_cast<const wchar_t*>(self->field_001c);
    g_CreateDirectoryW(directory, nullptr);

    char result = 0;
    directory =
        self->field_0030 < 8
            ? reinterpret_cast<const wchar_t*>(&self->field_001c)
            : reinterpret_cast<const wchar_t*>(self->field_001c);

    const auto setCurrentDirectory = g_SetCurrentDirectoryW;
    if (setCurrentDirectory(directory))
        result = self->vSlot30(
            reinterpret_cast<int>(CSF_STR(0x0083FFA0, "Options")));

    setCurrentDirectory(previousDirectory);
    StoreOptionsByte104(previousOption104);
    SetOptionsByteAt40(previousOption40);
    CoreFiles_SetFileOption(previousFileOption);
    return result;
}

HOOK(0x0050B7D0, COptionsFile_LoadOptions, "auto");
