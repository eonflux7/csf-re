#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, S_006d5390_AppendEntry, 0x006D54A0, (void* self, void* edx, void* a3));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall S_006d4450_FindOrAddTexture(
    S_006d4450* self, void* /*edx*/, char* name, int /*unused*/)
{
    auto* entries = reinterpret_cast<S_006d5390*>(
        reinterpret_cast<uint8_t*>(self) + 4);

    uint32_t index = 0;
    uint32_t entryOffset = 0;
    int foundIndex = -1;

    for (;;)
    {
        const uint32_t begin = entries->field_0004;
        if (!begin)
            break;

        const int count =
            static_cast<int32_t>(entries->field_0008 - begin) / 60;
        if (index >= static_cast<uint32_t>(count))
            break;

        if (foundIndex != -1)
            return foundIndex;

        auto* entry = reinterpret_cast<uint8_t*>(
            self->field_0008 + entryOffset);
        const char* entryName;
        if (CSF_AT(uint32_t, entry, 0x18) < 0x10)
            entryName = reinterpret_cast<const char*>(entry + 4);
        else
            entryName = CSF_AT(const char*, entry, 0x04);

        if (entryName && name && !__stricmp(entryName, name))
        {
            entry = reinterpret_cast<uint8_t*>(
                self->field_0008 + entryOffset);
            if (!CSF_AT(uint32_t, entry, 0x28) &&
                CSF_AT(int32_t, entry, 0x1C) != -1)
            {
                CSF_AT(int32_t, entry, 0x1C) = -1;
            }
            foundIndex = static_cast<int>(index);
        }

        ++index;
        entryOffset += 60;
    }

    if (foundIndex != -1)
        return foundIndex;

    alignas(8) uint8_t entryBytes[0x44];
    C_0082d410& entryString =
        *reinterpret_cast<C_0082d410*>(entryBytes);

    CSF_AT(uint32_t, entryBytes, 0x14) = 0;
    CSF_AT(uint32_t, entryBytes, 0x20) = 0;
    CSF_AT(uint32_t, entryBytes, 0x24) = 0;
    CSF_AT(uint32_t, entryBytes, 0x28) = 0;
    CSF_AT(uint32_t, entryBytes, 0x30) = 0;
    CSF_AT(uint32_t, entryBytes, 0x34) = 0;
    CSF_AT(uint32_t, entryBytes, 0x38) = 0;
    CSF_AT(uint32_t, entryBytes, 0x18) = 15;
    reinterpret_cast<uint8_t*>(&entryString.field_0004)[0] = 0;
    CSF_AT(uint32_t, entryBytes, 0x2C) = 10;

    unsigned int length = 0;
    while (name[length])
        ++length;

    AssignStringRange_429B60(&entryString, nullptr, name, length);
    CSF_AT(int32_t, entryBytes, 0x1C) = -1;
    CSF_AT(uint32_t, entryBytes, 0x30) = 0;
    S_006d5390_AppendEntry(entries, nullptr, entryBytes);

    const uint32_t begin = entries->field_0004;
    const int count = begin
        ? static_cast<int32_t>(entries->field_0008 - begin) / 60
        : 0;
    const int newIndex = count - 1;

    if (CSF_AT(uint32_t, entryBytes, 0x20))
    {
        Mem_Free(reinterpret_cast<void*>(
            CSF_AT(uint32_t, entryBytes, 0x20)));
        CSF_AT(uint32_t, entryBytes, 0x20) = 0;
    }

    if (CSF_AT(uint32_t, entryBytes, 0x18) >= 0x10 &&
        entryString.field_0004)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(entryString.field_0004));
    }

    return newIndex;
}

HOOK(0x006D5560, S_006d4450_FindOrAddTexture, "auto");
