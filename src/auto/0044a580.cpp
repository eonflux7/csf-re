#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, FindEntryIndex, 0x00448830,
                (void* self, void* edx, int a2));
static CSF_FUNC(uint32_t*, __fastcall, AppendEntry, 0x0044A390,
                (void* self, void* edx, int* a3));
static CSF_FUNC(int, __fastcall, DestroyTemporaryEntry, 0x00448560,
                (void* self, void* edx));

static int __fastcall S_00448830_GetOrCreateEntry(
    S_00448830* self, void* /*edx*/, int key)
{
    int index = FindEntryIndex(self, nullptr, key);
    if (index == -1)
    {
        const uint32_t begin = self->field_0018;
        index = begin
            ? static_cast<int32_t>(CSF_AT(uint32_t, self, 0x1C) - begin) / 20
            : 0;

        alignas(8) uint8_t entryBytes[0x14];
        int* entry = reinterpret_cast<int*>(entryBytes);
        entry[1] = 0;
        entry[2] = 0;
        entry[3] = 0;
        entry[4] = 10;
        entry[0] = key;

        AppendEntry(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr, entry);
        DestroyTemporaryEntry(entry, nullptr);
    }

    return static_cast<int32_t>(
        self->field_0018 + 20u * static_cast<uint32_t>(index));
}

HOOK(0x0044A580, S_00448830_GetOrCreateEntry, "auto");
