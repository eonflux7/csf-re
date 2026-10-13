#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __stdcall, NetProfile_WriteProfileData, 0x00518660, (int a1, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));

using GetCurrentDirectoryWFn = std::uint32_t (__stdcall*)(std::uint32_t, wchar_t*);
using SetCurrentDirectoryWFn = int (__stdcall*)(const wchar_t*);
CSF_GLOBAL(GetCurrentDirectoryWFn, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWFn, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall NetProfile_SaveProfile(void* self, void* /*edx*/,
                                              int profileIndex, S_00518780* profile)
{
    if (profileIndex < 0 || profileIndex >= 10)
    {
        CSF_AT(int, self, 0x1704) = 12;
        return 0;
    }

    char saved = 0;
    alignas(8) std::uint8_t filenameBytes[0x1C];
    C_0082d410& filename = *reinterpret_cast<C_0082d410*>(filenameBytes);

    CSF_AT(std::uint32_t, &filename, 0x18) = 15;
    CSF_AT(std::uint32_t, &filename, 0x14) = 0;
    *reinterpret_cast<char*>(&filename.field_0004) = 0;

    AssignStringRange_429B60(&filename, nullptr,
                            const_cast<char*>(CSF_STR(0x00840B8C, "PERF")), 4);
    StringAppendFill(&filename, nullptr, 1,
                     static_cast<char>(profileIndex / 10 + 48));
    StringAppendFill(&filename, nullptr, 1,
                     static_cast<char>(profileIndex % 10 + 48));
    Profiles_AppendStringRange(&filename, 0,
                              const_cast<char*>(CSF_STR(0x00840B84, ".PLB")), 4);

    alignas(8) std::uint8_t directoryBytes[0x804];
    wchar_t* originalDirectory = reinterpret_cast<wchar_t*>(directoryBytes);
    g_GetCurrentDirectoryW(0x400u, originalDirectory);

    const char originalOption = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* profileDirectory;
    if (CSF_AT(std::uint32_t, self, 0x1740) < 8u)
        profileDirectory = reinterpret_cast<const wchar_t*>(
            static_cast<std::uint8_t*>(self) + 0x172C);
    else
        profileDirectory = CSF_AT(const wchar_t*, self, 0x172C);

    if (g_SetCurrentDirectoryW(profileDirectory))
    {
        const char* path = CSF_AT(std::uint32_t, &filename, 0x18) < 0x10
            ? reinterpret_cast<const char*>(&filename.field_0004)
            : reinterpret_cast<const char*>(filename.field_0004);

        if (static_cast<unsigned char>(CoreFiles_CheckPageAvailability(path, 0)))
        {
            path = CSF_AT(std::uint32_t, &filename, 0x18) < 0x10
                ? reinterpret_cast<const char*>(&filename.field_0004)
                : reinterpret_cast<const char*>(filename.field_0004);

            void* file = CoreFiles_OpenSceneFile(
                path, const_cast<char*>(CSF_STR(0x00840044, "w")), 0);
            if (file)
            {
                if (NetProfile_WriteProfileData(reinterpret_cast<int>(file), profile))
                {
                    CSF_AT(int, self, 0x1704) = 8;
                }
                else
                {
                    saved = 1;
                    CSF_AT(std::uint8_t, self, 0x1708) = 1;
                }
                CloseSceneFile(file);
            }
            else
            {
                CSF_AT(int, self, 0x1704) = 8;
            }
        }
        else
        {
            CSF_AT(int, self, 0x1704) = 5;
        }
    }
    else
    {
        CSF_AT(int, self, 0x1704) = 5;
    }

    g_SetCurrentDirectoryW(originalDirectory);
    StoreOptionsByte104(originalOption);

    if (CSF_AT(std::uint32_t, &filename, 0x18) >= 0x10)
    {
        if (filename.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(filename.field_0004));
    }

    return saved;
}

HOOK(0x00517B80, NetProfile_SaveProfile, "auto");
