#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __fastcall, S_00517800_QueryFirstFreeSlot, 0x00517800, (S_00517800* self, void*));
CSF_FUNC(int, __stdcall, NetProfile_ReadProfile_518660, 0x00518660, (void* stream, S_00518780* profile));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(void, __cdecl, CoreFiles_CloseSceneFile, 0x00697980, (int file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));

using CreateDirectoryWPointer = int (__stdcall*)(const wchar_t*, void*);
using GetCurrentDirectoryWPointer = std::uint32_t (__stdcall*)(std::uint32_t, wchar_t*);
using SetCurrentDirectoryWPointer = int (__stdcall*)(const wchar_t*);

CSF_GLOBAL(CreateDirectoryWPointer, g_CreateDirectoryW, 0x0082D218);
CSF_GLOBAL(GetCurrentDirectoryWPointer, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWPointer, g_SetCurrentDirectoryW, 0x0082D190);

static bool __fastcall S_00517800_SaveProfile(
    S_00517800* self, void* /*edx*/, int profileAddress)
{
    const wchar_t* directory = self->field_1740 < 8
        ? reinterpret_cast<const wchar_t*>(&self->field_172c)
        : reinterpret_cast<const wchar_t*>(self->field_172c);

    int firstFreeSlot = 0;
    g_CreateDirectoryW(directory, nullptr);
    self->field_1704 = 0;

    const char previousFileOption = GetOptionsByteAt2();
    if (previousFileOption)
        CoreFiles_SetFileOption(0);

    bool success = false;
    if (self->field_1708)
    {
        self->field_1704 = 1;
    }
    else if (!profileAddress)
    {
        self->field_1704 = 11;
    }
    else
    {
        firstFreeSlot = S_00517800_QueryFirstFreeSlot(self, nullptr);
        success = firstFreeSlot != -1;
        if (!success)
            self->field_1704 = 2;
    }

    alignas(8) std::uint8_t directoryBytes[0x808];
    auto* previousDirectory = reinterpret_cast<wchar_t*>(directoryBytes);
    g_GetCurrentDirectoryW(0x400, previousDirectory);

    const char previousDirectoryOption = GetOptionsByte104();
    StoreOptionsByte104(1);

    alignas(8) std::uint8_t filenameBytes[0x1C];
    auto& filename = *reinterpret_cast<C_0082d410*>(filenameBytes);
    CSF_AT(std::uint32_t, &filename, 0x18) = 15;
    CSF_AT(std::uint32_t, &filename, 0x14) = 0;
    reinterpret_cast<char*>(&filename.field_0004)[0] = 0;

    directory = self->field_1740 < 8
        ? reinterpret_cast<const wchar_t*>(&self->field_172c)
        : reinterpret_cast<const wchar_t*>(self->field_172c);

    if (g_SetCurrentDirectoryW(directory) && success)
    {
        AssignStringRange_429B60(
            &filename, nullptr, const_cast<char*>(CSF_STR(0x00840B8C, "PERF")), 4);
        StringAppendFill(
            &filename, nullptr, 1, static_cast<char>(firstFreeSlot / 10 + 48));
        StringAppendFill(
            &filename, nullptr, 1, static_cast<char>(firstFreeSlot % 10 + 48));
        Profiles_AppendStringRange(
            &filename, 0, const_cast<char*>(CSF_STR(0x00840B84, ".PLB")), 4);

        const char* path = CSF_AT(std::uint32_t, &filename, 0x18) < 0x10
            ? reinterpret_cast<const char*>(&filename.field_0004)
            : reinterpret_cast<const char*>(filename.field_0004);
        success = static_cast<std::uint8_t>(
            CoreFiles_CheckPageAvailability(path, 0)) == 0;
        if (!success)
            self->field_1704 = 6;
    }

    if (success)
    {
        const char* path = CSF_AT(std::uint32_t, &filename, 0x18) < 0x10
            ? reinterpret_cast<const char*>(&filename.field_0004)
            : reinterpret_cast<const char*>(filename.field_0004);
        self->field_1708 = 1;

        void* stream = CoreFiles_OpenSceneFile(
            path, const_cast<char*>(CSF_STR(0x00840044, "w")), 0);
        success = stream != nullptr;
        if (stream)
        {
            success = NetProfile_ReadProfile_518660(
                stream, reinterpret_cast<S_00518780*>(profileAddress)) == 0;
            if (!success)
            {
                self->field_1704 = 8;
            }
            else
            {
                self->field_1708 = 1;
                AssignStringSubstring(
                    reinterpret_cast<std::uint8_t*>(self) + 0x170C,
                    nullptr, &filename, 0, static_cast<std::uint32_t>(-1));
            }
        }
        else
        {
            self->field_1704 = 5;
        }

        if (stream)
            CoreFiles_CloseSceneFile(reinterpret_cast<int>(stream));
    }

    g_SetCurrentDirectoryW(previousDirectory);
    StoreOptionsByte104(previousDirectoryOption);
    CoreFiles_SetFileOption(previousFileOption);

    if (CSF_AT(std::uint32_t, &filename, 0x18) >= 0x10 &&
        filename.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(filename.field_0004));
    }
    return success;
}

HOOK(0x00517930, S_00517800_SaveProfile, "auto");
