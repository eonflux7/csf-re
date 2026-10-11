#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

using GetCurrentDirectoryWFn = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using CreateDirectoryWFn = int (__stdcall*)(const wchar_t*, void*);
using SetCurrentDirectoryWFn = int (__stdcall*)(const wchar_t*);

CSF_GLOBAL(GetCurrentDirectoryWFn, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(CreateDirectoryWFn, g_CreateDirectoryW, 0x0082D218);
CSF_GLOBAL(SetCurrentDirectoryWFn, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall CProfileManager_SaveProfiles(
    CProfileManager* self, void* /*edx*/)
{
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC))
        return 1;

    const char previousFileOption = GetOptionsByteAt2();
    if (previousFileOption)
        SceneLoader_SetFileOption(0);

    const char previousOption40 = GetOptionsByte40();
    if (!previousOption40)
        SetOptionsByteAt40(1);

    alignas(8) uint8_t directoryBytes[0x80C];
    auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);
    g_GetCurrentDirectoryW(0x400u, previousDirectory);

    const char previousOption104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* profileDirectory =
        self->field_0064 < 8
            ? reinterpret_cast<const wchar_t*>(&self->field_0050)
            : reinterpret_cast<const wchar_t*>(self->field_0050);
    g_CreateDirectoryW(profileDirectory, nullptr);

    char result = 0;
    profileDirectory =
        self->field_0064 < 8
            ? reinterpret_cast<const wchar_t*>(&self->field_0050)
            : reinterpret_cast<const wchar_t*>(self->field_0050);

    const auto setCurrentDirectory = g_SetCurrentDirectoryW;
    if (setCurrentDirectory(profileDirectory))
    {
        result = self->vSlot80(
            reinterpret_cast<int>(CSF_STR(0x008409B4, "./Profiles")),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
    }

    setCurrentDirectory(previousDirectory);
    StoreOptionsByte104(previousOption104);
    SetOptionsByteAt40(previousOption40);
    SceneLoader_SetFileOption(previousFileOption);
    return result;
}

HOOK(0x005149A0, CProfileManager_SaveProfiles, "auto");
