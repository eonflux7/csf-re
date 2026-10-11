#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, Profiles_ResetEntries, 0x005154B0, (int* a1));
CSF_FUNC(int, __fastcall, ComputeChildHandleFromSlot, 0x00515BD0, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

using GetCurrentDirectoryWProc = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const wchar_t*);

CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall CProfileManager_LoadProfiles(
    CProfileManager* self, void* /*edx*/)
{
    char result = 1;

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC) &&
        !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x21))
    {
        auto* entries = reinterpret_cast<uint8_t*>(self) + 4;
        Profiles_ResetEntries(reinterpret_cast<int*>(entries));

        const char oldFileOption = GetOptionsByteAt2();
        if (oldFileOption)
            SceneLoader_SetFileOption(0);

        const char oldOption40 = GetOptionsByte40();
        if (!oldOption40)
            SetOptionsByteAt40(1);

        alignas(8) uint8_t directoryBytes[0x810];
        auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);
        g_GetCurrentDirectoryW(0x400u, previousDirectory);

        const char oldOption104 = GetOptionsByte104();
        StoreOptionsByte104(1);

        const wchar_t* profileDirectory;
        if (self->field_0064 < 8)
            profileDirectory = reinterpret_cast<const wchar_t*>(&self->field_0050);
        else
            profileDirectory = reinterpret_cast<const wchar_t*>(self->field_0050);

        const auto setCurrentDirectory = g_SetCurrentDirectoryW;
        if (!setCurrentDirectory(profileDirectory) ||
            !self->vSlot7C(
                reinterpret_cast<int>(CSF_STR(0x008409B4, "./Profiles")),
                entries))
        {
            result = 0;
        }

        setCurrentDirectory(previousDirectory);
        StoreOptionsByte104(oldOption104);
        SetOptionsByteAt40(oldOption40);
        SceneLoader_SetFileOption(oldFileOption);
    }

    const uint32_t begin = self->field_0008;
    if (begin)
    {
        const int32_t byteCount =
            static_cast<int32_t>(self->field_000c - begin);

        // Signed division by the 80-byte entry size, using the original reciprocal.
        const int64_t product = int64_t{byteCount} * 0x66666667;
        const int32_t roundedDownCount = static_cast<int32_t>(product >> 37);
        const int32_t count = roundedDownCount + (roundedDownCount < 0);

        if (count && ComputeChildHandleFromSlot(self, nullptr) == -1)
        {
            CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x4C) = 1;
        }
    }

    return result;
}

HOOK(0x00515500, CProfileManager_LoadProfiles, "auto");
