#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, StringEntries_AppendEntry, 0x006C99E0, (void* self, void* edx, int a3));

static int __fastcall C_00860954_FindOrAddName(
    C_00860954* self, void* /*edx*/, char* name, int16_t* value)
{
    const int existingIndex = static_cast<int>(self->vSlot18(name));
    if (existingIndex != -1)
        return existingIndex;

    alignas(8) uint8_t entryBytes[0x24];
    C_0082d410& entry = *reinterpret_cast<C_0082d410*>(entryBytes);

    CSF_AT(uint32_t, entryBytes, 0x18) = 15;
    CSF_AT(uint32_t, entryBytes, 0x14) = 0;
    reinterpret_cast<uint8_t*>(&entry.field_0004)[0] = 0;
    CSF_AT(int16_t, entryBytes, 0x20) = 0;
    CSF_AT(uint32_t, entryBytes, 0x1C) = 0;

    AssignStringRange_429B60(
        &entry, nullptr, name, static_cast<unsigned int>(std::strlen(name)));

    CSF_AT(int16_t, entryBytes, 0x20) = *value;
    StringEntries_AppendEntry(
        reinterpret_cast<uint8_t*>(self) + 4,
        nullptr,
        reinterpret_cast<int>(&entry));

    const uint32_t begin = self->field_0008;
    int count = 0;
    if (begin != 0)
    {
        const uint32_t end = CSF_AT(uint32_t, self, 0x0C);
        count = static_cast<int32_t>(end - begin) / 36;
    }
    const int addedIndex = count - 1;

    if (CSF_AT(uint32_t, entryBytes, 0x18) >= 0x10)
    {
        if (entry.field_0004 != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(entry.field_0004));
    }

    return addedIndex;
}

HOOK(0x006C9AA0, C_00860954_FindOrAddName, "auto");
