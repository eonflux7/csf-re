#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00840af8_PrepareEntry, 0x005172C0,
         (void* self, void* edx, unsigned int* a2));

static char __fastcall C_00840af8_MarkLastEntryActive(
    C_00840af8* self, void* /*edx*/, unsigned int* entryData)
{
    if (!C_00840af8_PrepareEntry(self, nullptr, entryData))
        return 0;

    uint32_t count = self->field_0008;
    if (count)
        count = static_cast<int32_t>(self->field_000c - count) / 80;

    uint32_t offset = 0;
    while (count)
    {
        void* entry = reinterpret_cast<void*>(self->field_0008 + offset);
        offset += 80;
        --count;
        CSF_AT(uint8_t, entry, 76) = 0;
    }

    count = self->field_0008;
    if (count)
        count = static_cast<int32_t>(self->field_000c - count) / 80;

    void* lastEntry =
        reinterpret_cast<void*>(80 * count + self->field_0008 - 80);
    CSF_AT(uint8_t, lastEntry, 76) = 1;

    count = self->field_0008;
    if (count)
        count = static_cast<int32_t>(self->field_000c - count) / 80;

    lastEntry = reinterpret_cast<void*>(80 * count + self->field_0008 - 80);
    CSF_AT(uint8_t, lastEntry, 77) = 1;
    return 1;
}

HOOK(0x005173C0, C_00840af8_MarkLastEntryActive, "auto");
