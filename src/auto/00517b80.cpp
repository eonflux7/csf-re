#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __stdcall, NetProfile_ReadProfile_518660, 0x00518660, (void* stream, S_00518780* profile));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(void, __cdecl, CoreFiles_CloseSceneFile, 0x00697980, (int file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));

using GetCurrentDirectoryWProc = uint32_t (__stdcall*)(uint32_t, wchar_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const wchar_t*);
CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);

static char __fastcall NetProfile_SaveProfile(
    void* self, void* /*edx*/, int profileNumber, S_00518780* profile)
{
    if (static_cast<unsigned int>(profileNumber) > 9) {
        CSF_AT(int, self, 0x1704) = 12;
        return 0;
    }

    alignas(8) uint8_t directoryBytes[0x804];
    wchar_t* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);

    alignas(8) uint8_t filenameBytes[0x1C];
    C_0082d410& filename = *reinterpret_cast<C_0082d410*>(filenameBytes);
    CSF_AT(uint32_t, &filename, 0x18) = 15;
    CSF_AT(uint32_t, &filename, 0x14) = 0;
    CSF_AT(uint8_t, &filename, 0x4) = 0;

    char saved = 0;
    AssignStringRange_429B60(
        &filename, nullptr, const_cast<char*>(CSF_STR(0x00840B8C, "PERF")), 4);
    StringAppendFill(&filename, nullptr, 1, static_cast<char>(profileNumber / 10 + 48));
    StringAppendFill(&filename, nullptr, 1, static_cast<char>(profileNumber % 10 + 48));
    Profiles_AppendStringRange(
        &filename, 0, const_cast<char*>(CSF_STR(0x00840B84, ".PLB")), 4);

    g_GetCurrentDirectoryW(0x400u, previousDirectory);
    char previousOption = GetOptionsByte104();
    StoreOptionsByte104(1);

    const wchar_t* profileDirectory =
        CSF_AT(uint32_t, self, 0x1740) < 8u
            ? reinterpret_cast<const wchar_t*>(static_cast<uint8_t*>(self) + 0x172C)
            : reinterpret_cast<const wchar_t*>(CSF_AT(uint32_t, self, 0x172C));

    if (!g_SetCurrentDirectoryW(profileDirectory)) {
        CSF_AT(int, self, 0x1704) = 5;
    } else if (static_cast<uint8_t>(CoreFiles_CheckPageAvailability(
                   CSF_AT(uint32_t, &filename, 0x18) < 0x10
                       ? reinterpret_cast<const char*>(&filename.field_0004)
                       : reinterpret_cast<const char*>(filename.field_0004),
                   0))) {
        void* stream = CoreFiles_OpenSceneFile(
            CSF_AT(uint32_t, &filename, 0x18) < 0x10
                ? reinterpret_cast<const char*>(&filename.field_0004)
                : reinterpret_cast<const char*>(filename.field_0004),
            const_cast<char*>(CSF_STR(0x00840044, "w")), 0);

        if (stream) {
            if (NetProfile_ReadProfile_518660(stream, profile)) {
                CSF_AT(int, self, 0x1704) = 8;
            } else {
                saved = 1;
                CSF_AT(uint8_t, self, 0x1708) = 1;
            }
            CoreFiles_CloseSceneFile(static_cast<int>(reinterpret_cast<uintptr_t>(stream)));
        } else {
            CSF_AT(int, self, 0x1704) = 8;
        }
    } else {
        CSF_AT(int, self, 0x1704) = 5;
    }

    g_SetCurrentDirectoryW(previousDirectory);
    StoreOptionsByte104(previousOption);

    if (CSF_AT(uint32_t, &filename, 0x18) >= 0x10) {
        if (filename.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(filename.field_0004));
    }
    return saved;
}

HOOK(0x00517B80, NetProfile_SaveProfile, "auto");
