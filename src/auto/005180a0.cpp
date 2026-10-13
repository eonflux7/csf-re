#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __stdcall, NetProfile_ReadProfile_518530, 0x00518530, (void* stream, S_00518780* profile));
CSF_FUNC(int, __fastcall, S_00518780_IsFieldEqualToQueryResult, 0x00518860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00518780_ResetInnerObject, 0x00518890, (S_00518780* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00518780_Construct, 0x005188D0, (S_00518780* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, Options_SetFlag1, 0x00697300, (char a1));
CSF_FUNC(char, __cdecl, GetOptionsByte1, 0x00697320, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(bool, __cdecl, CoreFiles_DeleteFile, 0x006976C0, (const char* path));
CSF_FUNC(void, __cdecl, CoreFiles_CloseSceneFile, 0x00697980, (int file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));

using GetCurrentDirectoryWProc = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const wchar_t*);

CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall NetProfile_RemoveInvalidProfileFiles(void* self, void* /*edx*/)
{
    alignas(8) uint8_t directoryBytes[0x800];
    auto* savedDirectory = reinterpret_cast<wchar_t*>(directoryBytes);

    alignas(8) uint8_t profileBytes[0x254];
    auto& profile = *reinterpret_cast<S_00518780*>(profileBytes);

    // Includes the inline characters, length, and capacity beyond the declared type.
    alignas(8) uint8_t filenameBytes[0x28];
    auto& filename = *reinterpret_cast<C_0082d410*>(filenameBytes);

    g_GetCurrentDirectoryW(0x400u, savedDirectory);
    const char savedOptions104 = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* profileDirectory;
    if (CSF_AT(uint32_t, self, 0x1740) < 8u)
        profileDirectory = reinterpret_cast<const wchar_t*>(
            static_cast<uint8_t*>(self) + 0x172C);
    else
        profileDirectory = reinterpret_cast<const wchar_t*>(
            CSF_AT(uint32_t, self, 0x172C));

    const auto setCurrentDirectory = g_SetCurrentDirectoryW;
    if (!setCurrentDirectory(profileDirectory))
    {
        setCurrentDirectory(savedDirectory);
        StoreOptionsByte104(savedOptions104);
        return 0;
    }

    const char savedFileOption = GetOptionsByteAt2();
    if (savedFileOption)
        CoreFiles_SetFileOption(0);

    int profileIndex = 0;
    char removedAny = 0;
    CSF_AT(uint32_t, self, 0x1704) = 0;
    const char savedOptions1 = GetOptionsByte1();
    Options_SetFlag1(0);

    do
    {
        CSF_AT(uint32_t, &filename, 0x18) = 15;
        CSF_AT(uint32_t, &filename, 0x14) = 0;
        reinterpret_cast<char*>(&filename.field_0004)[0] = 0;

        AssignStringRange_429B60(
            &filename, nullptr, const_cast<char*>(CSF_STR(0x00840B8C, "PERF")), 4);
        StringAppendFill(
            &filename, nullptr, 1, static_cast<char>(profileIndex / 10 + 48));
        StringAppendFill(
            &filename, nullptr, 1, static_cast<char>(profileIndex % 10 + 48));
        Profiles_AppendStringRange(
            &filename, 0, const_cast<char*>(CSF_STR(0x00840B84, ".PLB")), 4);

        S_00518780_Construct(&profile, nullptr);

        const char* path = reinterpret_cast<const char*>(filename.field_0004);
        if (CSF_AT(uint32_t, &filename, 0x18) < 0x10)
            path = reinterpret_cast<const char*>(&filename.field_0004);

        void* stream = CoreFiles_OpenSceneFile(
            path, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);

        if (stream)
        {
            bool invalid = NetProfile_ReadProfile_518530(stream, &profile) != 0;
            if (!invalid)
            {
                invalid = static_cast<uint8_t>(
                    S_00518780_IsFieldEqualToQueryResult(&profile, nullptr)) == 0;
            }

            CoreFiles_CloseSceneFile(reinterpret_cast<int>(stream));
            if (invalid)
            {
                path = reinterpret_cast<const char*>(filename.field_0004);
                removedAny = 1;
                if (CSF_AT(uint32_t, &filename, 0x18) < 0x10)
                    path = reinterpret_cast<const char*>(&filename.field_0004);
                CoreFiles_DeleteFile(path);
            }
        }

        S_00518780_ResetInnerObject(&profile, nullptr);
        if (CSF_AT(uint32_t, &filename, 0x18) >= 0x10)
        {
            if (filename.field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(filename.field_0004));
        }

        ++profileIndex;
        CSF_AT(uint32_t, &filename, 0x18) = 15;
        CSF_AT(uint32_t, &filename, 0x14) = 0;
        reinterpret_cast<char*>(&filename.field_0004)[0] = 0;
    }
    while (profileIndex < 10);

    Options_SetFlag1(savedOptions1);
    CSF_AT(uint8_t, self, 0x1708) = 1;
    g_SetCurrentDirectoryW(savedDirectory);
    StoreOptionsByte104(savedOptions104);
    CoreFiles_SetFileOption(savedFileOption);
    return removedAny;
}

HOOK(0x005180A0, NetProfile_RemoveInvalidProfileFiles, "auto");
