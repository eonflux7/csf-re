#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));
CSF_FUNC(int, __cdecl, Profiles_CombinePath, 0x00512920, (void* a1, void* a2, void* a3));
CSF_FUNC(int, __cdecl, Profiles_DestroyEntryRange, 0x00515080, (int a1, int a2));
CSF_FUNC(void*, __cdecl, Profiles_MoveEntryRange, 0x00515160, (void* a2, void* arg4, void* a3));
CSF_FUNC(int, __fastcall, ComputeChildHandleFromSlot, 0x00515BD0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, StoreOptionsByte104, 0x006973C0, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte104, 0x006973E0, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

using GetCurrentDirectoryWProc = uint32_t (__stdcall*)(uint32_t, char16_t*);
using SetCurrentDirectoryWProc = int (__stdcall*)(const char16_t*);
using FindFirstFileAProc = void* (__stdcall*)(const char*, void*);
using FindNextFileAProc = int (__stdcall*)(void*, void*);
using FindCloseProc = int (__stdcall*)(void*);
using DeleteFileAProc = int (__stdcall*)(const char*);
using RemoveDirectoryAProc = int (__stdcall*)(const char*);

CSF_GLOBAL(GetCurrentDirectoryWProc, g_GetCurrentDirectoryW, 0x0082D21C);
CSF_GLOBAL(SetCurrentDirectoryWProc, g_SetCurrentDirectoryW, 0x0082D190);
CSF_GLOBAL(FindFirstFileAProc, g_FindFirstFileA, 0x0082D0DC);
CSF_GLOBAL(FindNextFileAProc, g_FindNextFileA, 0x0082D184);
CSF_GLOBAL(FindCloseProc, g_FindClose, 0x0082D18C);
CSF_GLOBAL(DeleteFileAProc, g_DeleteFileA, 0x0082D210);
CSF_GLOBAL(RemoveDirectoryAProc, g_RemoveDirectoryA, 0x0082D04C);

static int Profiles_CountEntries(uint32_t byteDistance)
{
    // Signed division by the 80-byte entry size, using the original reciprocal.
    const int64_t product =
        static_cast<int64_t>(static_cast<int32_t>(byteDistance)) * 0x66666667;
    const int32_t quotient = static_cast<int32_t>(product >> 32) >> 5;
    return quotient + static_cast<int>(static_cast<uint32_t>(quotient) >> 31);
}

static char __fastcall CProfileManager_DeleteProfile(
    CProfileManager* self, void* /*edx*/, unsigned int profileIndex)
{
    if (!self->field_0008 ||
        profileIndex >= static_cast<unsigned int>(Profiles_CountEntries(
            CSF_AT(uint32_t, self, 0x0C) - self->field_0008)))
        return 0;

    alignas(8) uint8_t directoryBytes[0x800];
    auto* originalDirectory = reinterpret_cast<char16_t*>(directoryBytes);
    alignas(8) uint8_t findDataBytes[0x140];
    alignas(8) uint8_t temporaryBytes[0x1C];
    C_0082d410& temporary = *reinterpret_cast<C_0082d410*>(temporaryBytes);
    alignas(8) uint8_t pathBytes[0x24];
    C_0082d410& path = *reinterpret_cast<C_0082d410*>(pathBytes);
    alignas(8) uint8_t numberBytes[0xC];
    auto* number = reinterpret_cast<char*>(numberBytes);

    g_GetCurrentDirectoryW(0x400u, originalDirectory);
    const char savedOptions = GetOptionsByte104();
    StoreOptionsByte104(1);

    const char16_t* profileDirectory =
        self->field_0048 < 8
            ? reinterpret_cast<const char16_t*>(&self->field_0034)
            : reinterpret_cast<const char16_t*>(self->field_0034);
    g_SetCurrentDirectoryW(profileDirectory);

    CSF_AT(uint32_t, &path, 0x18) = 15;
    CSF_AT(uint32_t, &path, 0x14) = 0;
    reinterpret_cast<char*>(&path.field_0004)[0] = 0;
    AssignStringRange_429B60(
        &path, nullptr, const_cast<char*>(CSF_STR(0x00840888, "Games")), 5);

    const uint32_t entryOffset = 80 * profileIndex;
    _sprintf(number, CSF_STR(0x0083090C, "%d"),
             CSF_AT(int, reinterpret_cast<void*>(self->field_0008 + entryOffset), 0x48));
    Profiles_AppendStringRange(
        &path, 0, number, static_cast<unsigned int>(std::strlen(number)));
    StringAppendFill(&path, nullptr, 1, 47);

    int nextFile = 1;
    void* combined = reinterpret_cast<void*>(Profiles_CombinePath(
        &temporary, &path, const_cast<char*>(CSF_STR(0x008408B8, "*.sav"))));
    const char* searchPath =
        CSF_AT(uint32_t, combined, 0x18) < 0x10u
            ? reinterpret_cast<const char*>(combined) + 4
            : CSF_AT(const char*, combined, 4);
    void* findHandle = g_FindFirstFileA(searchPath, findDataBytes);

    if (CSF_AT(uint32_t, &temporary, 0x18) >= 0x10 &&
        temporary.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(temporary.field_0004));

    if (findHandle != reinterpret_cast<void*>(static_cast<uintptr_t>(-1)))
    {
        while (nextFile)
        {
            combined = reinterpret_cast<void*>(Profiles_CombinePath(
                &temporary, &path, findDataBytes + 0x2C));
            const char* filePath =
                CSF_AT(uint32_t, combined, 0x18) < 0x10u
                    ? reinterpret_cast<const char*>(combined) + 4
                    : CSF_AT(const char*, combined, 4);
            g_DeleteFileA(filePath);

            if (CSF_AT(uint32_t, &temporary, 0x18) >= 0x10 &&
                temporary.field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(temporary.field_0004));

            CSF_AT(uint32_t, &temporary, 0x18) = 15;
            CSF_AT(uint32_t, &temporary, 0x14) = 0;
            reinterpret_cast<char*>(&temporary.field_0004)[0] = 0;
            nextFile = g_FindNextFileA(findHandle, findDataBytes);
        }
    }

    g_FindClose(findHandle);
    const char* gamesDirectory =
        CSF_AT(uint32_t, &path, 0x18) < 0x10
            ? reinterpret_cast<const char*>(&path.field_0004)
            : reinterpret_cast<const char*>(path.field_0004);
    g_RemoveDirectoryA(gamesDirectory);

    if (profileIndex ==
        static_cast<unsigned int>(ComputeChildHandleFromSlot(self, nullptr)))
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* activeObject = CSF_AT(void*, manager, 4);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, activeObject, 0)[0x10 / 4])(activeObject, nullptr);
    }

    void* erasedEntry = reinterpret_cast<void*>(self->field_0008 + entryOffset);
    Profiles_MoveEntryRange(
        reinterpret_cast<uint8_t*>(erasedEntry) + 80,
        reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C)),
        erasedEntry);

    Profiles_DestroyEntryRange(
        static_cast<int>(CSF_AT(uint32_t, self, 0x0C) - 80),
        static_cast<int>(CSF_AT(uint32_t, self, 0x0C)));

    CSF_AT(uint32_t, self, 0x0C) -= 80;
    if (self->field_0008 &&
        Profiles_CountEntries(
            CSF_AT(uint32_t, self, 0x0C) - self->field_0008) != 0)
    {
        if (ComputeChildHandleFromSlot(self, nullptr) == -1)
            CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x4C) = 1;
    }

    g_SetCurrentDirectoryW(originalDirectory);
    StoreOptionsByte104(savedOptions);
    const char result = static_cast<char>(
        reinterpret_cast<int (__fastcall*)(CProfileManager*, void*)>(
            CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr));

    if (CSF_AT(uint32_t, &path, 0x18) >= 0x10 && path.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(path.field_0004));

    return result;
}

HOOK(0x00515210, CProfileManager_DeleteProfile, "auto");
