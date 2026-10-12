#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_00448830_FindEntryIndex(
    S_00448830* self, void* /*edx*/, int key)
{
    const uint32_t begin = self->field_0018;
    uint32_t entryCount = 0;
    if (begin != 0) {
        const int32_t byteSpan =
            static_cast<int32_t>(self->field_001c - begin);
        entryCount = static_cast<uint32_t>(byteSpan / 20);
    }

    if (entryCount == 0)
        return -1;

    const int32_t* entry =
        reinterpret_cast<const int32_t*>(self->field_0018);
    uint32_t index = 0;
    while (*entry != key) {
        ++index;
        entry += 5;
        if (index >= entryCount)
            return -1;
    }
    return static_cast<int>(index);
}

HOOK(0x00448830, S_00448830_FindEntryIndex, "auto");
