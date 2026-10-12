#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int*, __fastcall, Campaign_EraseEntry, 0x0043EFC0, (uint32_t* a1, int a2, int* a3, int a4));
CSF_FUNC(void**, __fastcall, Campaign_FindEntry, 0x0043F2A0, (void* self, void* edx, void** a2, unsigned int* a3));
CSF_FUNC(int, __fastcall, Campaign_InsertEntry, 0x00440C80, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static std::uint32_t __fastcall Campaign_ResolveHandleWithGlobal(
    void* self, void* /*edx*/, unsigned int* key, int handle)
{
    alignas(8) std::uint8_t entryBytes[0x20];
    alignas(8) std::uint8_t insertionBytes[0x20];
    alignas(8) std::uint8_t keyBytes[0x1C];
    alignas(8) std::uint8_t insertionResultBytes[0x4];
    alignas(8) std::uint8_t iteratorBytes[0x4];

    auto& entryString = *reinterpret_cast<C_0082d410*>(entryBytes);
    auto& insertionString = *reinterpret_cast<C_0082d410*>(insertionBytes);
    auto& keyString = *reinterpret_cast<C_0082d410*>(keyBytes);

    auto* entries = static_cast<std::uint8_t*>(self) + 4;
    void* found = *Campaign_FindEntry(
        entries, nullptr, reinterpret_cast<void**>(iteratorBytes), key);
    if (found != CSF_AT(void*, self, 0x8))
    {
        Campaign_EraseEntry(
            reinterpret_cast<uint32_t*>(entries), 0,
            reinterpret_cast<int*>(iteratorBytes),
            reinterpret_cast<int>(found));
    }

    CSF_AT(std::uint32_t, keyBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, keyBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&keyString.field_0004)[0] = 0;
    AssignStringSubstring(&keyString, nullptr, key, 0, -1);

    CSF_AT(std::uint32_t, entryBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, entryBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&entryString.field_0004)[0] = 0;
    AssignStringSubstring(&entryString, nullptr, &keyString, 0, -1);
    CSF_AT(int, entryBytes, 0x1C) = handle;

    if (CSF_AT(std::uint32_t, keyBytes, 0x18) >= 0x10 &&
        keyString.field_0004 != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(keyString.field_0004));
    }

    CSF_AT(std::uint32_t, keyBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, keyBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&keyString.field_0004)[0] = 0;

    CSF_AT(std::uint32_t, insertionBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, insertionBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&insertionString.field_0004)[0] = 0;
    AssignStringSubstring(&insertionString, nullptr, &entryString, 0, -1);
    CSF_AT(int, insertionBytes, 0x1C) = CSF_AT(int, entryBytes, 0x1C);
    Campaign_InsertEntry(
        entries, nullptr, reinterpret_cast<int>(insertionResultBytes),
        reinterpret_cast<void**>(insertionBytes));

    std::uint32_t result = CSF_AT(std::uint32_t, insertionBytes, 0x18);
    if (result >= 0x10)
    {
        result = insertionString.field_0004;
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    const bool releaseEntry = CSF_AT(std::uint32_t, entryBytes, 0x18) >= 0x10;
    CSF_AT(std::uint32_t, insertionBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, insertionBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&insertionString.field_0004)[0] = 0;

    if (releaseEntry)
    {
        result = entryString.field_0004;
        if (result != 0)
            return Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    return result;
}

HOOK(0x00441220, Campaign_ResolveHandleWithGlobal, "auto");
