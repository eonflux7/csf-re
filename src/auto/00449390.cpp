#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, EntryTable_GrowStorage, 0x00448C60,
         (void* self, void* edx, int a2));

static int __fastcall EntryTable_FindOrAddEntry(
    void* self, void* /*edx*/, const void* source)
{
    const uint32_t originalCount = CSF_AT(uint32_t, self, 0x0C);
    int index = -1;

    if (originalCount != 0)
    {
        auto* entry = CSF_AT(uint8_t*, self, 0x04);
        const uint32_t key = CSF_AT(uint32_t, source, 0x04);

        for (uint32_t i = 0; i < originalCount; ++i, entry += 36)
        {
            if (CSF_AT(uint32_t, entry, 0x04) == key)
            {
                index = static_cast<int>(i);
                break;
            }
        }
    }

    if (index == -1)
    {
        const uint32_t value = CSF_AT(uint32_t, source, 0x08);
        const uint32_t key = CSF_AT(uint32_t, source, 0x04);
        index = static_cast<int>(originalCount);

        if (CSF_AT(uint32_t, self, 0x0C) >=
            CSF_AT(uint32_t, self, 0x08))
        {
            EntryTable_GrowStorage(
                static_cast<uint8_t*>(self) + 0x04,
                nullptr,
                CSF_AT(int, self, 0x10));
        }

        const uint32_t count = CSF_AT(uint32_t, self, 0x0C);
        auto* entry = CSF_AT(uint8_t*, self, 0x04) + 36 * count;
        CSF_AT(uint32_t, self, 0x0C) = count + 1;

        CSF_AT(uint32_t, entry, 0x08) = value;
        CSF_AT(uint32_t, entry, 0x04) = key;
        CSF_AT(uint32_t, entry, 0x0C) = 0;
        CSF_AT(int, entry, 0x14) = -1;
        CSF_AT(int, entry, 0x18) = -1;
        CSF_AT(uint32_t, entry, 0x1C) = 0;
        CSF_AT(uint32_t, entry, 0x20) = 0;
    }

    return static_cast<int>(
        CSF_AT(uint32_t, self, 0x04) + 36 * static_cast<uint32_t>(index));
}

HOOK(0x00449390, EntryTable_FindOrAddEntry, "auto");
