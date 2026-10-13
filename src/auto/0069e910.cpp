#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __cdecl, CoreTokenizer_ReplacePathCharacter, 0x00694BF0, (uint8_t* a1, char a2, char a3));
CSF_FUNC(bool, __cdecl, CoreFiles_RemoveBaseDirectoryPrefix, 0x00697470, (char* path));
CSF_FUNC(void**, __fastcall, C_0085edfc_Init, 0x006A29A0, (C_0085edfc* self, void* , uint32_t firstArg, uint32_t secondArg, uint32_t thirdArg));
CSF_FUNC(C_0085ee28*, __fastcall, C_0085ee28_Construct, 0x006A2E30, (C_0085ee28* self, void* , int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static uint32_t* __fastcall CoreFiles_FindFile(
    void* self, void* /*edx*/, const char* path)
{
    alignas(8) uint8_t pathBytes[0x100] = {};
    char* normalizedPath = reinterpret_cast<char*>(pathBytes);
    std::strcpy(normalizedPath, path);
    CoreFiles_RemoveBaseDirectoryPrefix(normalizedPath);
    CoreTokenizer_ReplacePathCharacter(pathBytes, 92, 47);

    const uint32_t entriesBegin = CSF_AT(uint32_t, self, 0x10);
    uint32_t entryCount = 0;
    if (entriesBegin)
    {
        const int32_t byteCount = static_cast<int32_t>(
            CSF_AT(uint32_t, self, 0x14) - entriesBegin);
        entryCount = static_cast<uint32_t>(byteCount / 44);
    }

    uint32_t entryIndex = 0;
    uint32_t entryOffset = 0;
    for (; entryIndex < entryCount; ++entryIndex, entryOffset += 44)
    {
        const auto* entry = reinterpret_cast<const uint8_t*>(
            CSF_AT(uint32_t, self, 0x10) + entryOffset);

        const char* entryName;
        if (CSF_AT(uint32_t, entry, 0x18) < 0x10)
            entryName = reinterpret_cast<const char*>(entry + 4);
        else
            entryName = CSF_AT(const char*, entry, 4);

        if (__stricmp(entryName, normalizedPath) == 0)
            break;
    }

    if (entryIndex >= entryCount)
        return nullptr;

    uint32_t* file = nullptr;
    if (CSF_AT(uint8_t, self, 0x3C))
    {
        auto* storage = reinterpret_cast<C_0085ee28*>(
            CoreMemory_AllocUntagged(9256));
        if (storage)
        {
            const auto* entry = reinterpret_cast<const uint8_t*>(
                CSF_AT(uint32_t, self, 0x10) + 44 * entryIndex);
            const int constructionArg = CSF_AT(int, self, 0x40);
            const uint32_t baseOffset = CSF_AT(uint32_t, self, 8);
            const uint32_t fileOffset = CSF_AT(uint32_t, entry, 0x1C);
            const int fileSize = CSF_AT(int, entry, 0x20);

            file = reinterpret_cast<uint32_t*>(C_0085ee28_Construct(
                storage, nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                static_cast<int>(baseOffset + fileOffset),
                fileSize, constructionArg));
        }
    }
    else
    {
        auto* storage = reinterpret_cast<C_0085edfc*>(
            CoreMemory_AllocUntagged(20));
        if (storage)
        {
            const auto* entry = reinterpret_cast<const uint8_t*>(
                CSF_AT(uint32_t, self, 0x10) + 44 * entryIndex);
            const uint32_t fileSize = CSF_AT(uint32_t, entry, 0x20);
            const uint32_t fileOffset = CSF_AT(uint32_t, entry, 0x1C);
            const uint32_t baseOffset = CSF_AT(uint32_t, self, 8);

            file = reinterpret_cast<uint32_t*>(C_0085edfc_Init(
                storage, nullptr,
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self)),
                baseOffset + fileOffset, fileSize));
        }
    }

    SetAllocationMode(3);
    const uint32_t openCount = CSF_AT(uint32_t, self, 0x34);
    const uint32_t openCapacity = CSF_AT(uint32_t, self, 0x30);
    auto* openFiles = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x2C);

    if (openCount >= openCapacity)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            openFiles, nullptr, openFiles->field_000c);
    }

    const uint32_t insertionIndex = openFiles->field_0008;
    auto* destination = reinterpret_cast<uint32_t*>(
        openFiles->field_0000 + 4 * insertionIndex);
    openFiles->field_0008 = insertionIndex + 1;
    *destination = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(file));
    SetAllocationMode(0);
    return file;
}

HOOK(0x0069E910, CoreFiles_FindFile, "auto");
