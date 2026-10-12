#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Finds the index of the 40-byte status-icon entry whose stored position
// (floats at +0x1C, +0x20, +0x24) equals the given 3-float position.
// Returns the entry index, or -1 if there are no entries or none match.
static int __fastcall S_00458980_FindEntryIndexByPosition(S_00458980* self, void* /*edx*/, float* position)
{
    uint32_t entryBegin = self->field_001c;
    uint32_t entryCount = 0;
    if (entryBegin != 0) {
        entryCount = (uint32_t)((int)(self->field_0020 - entryBegin) / 40);
    }
    if (entryCount == 0) {
        return -1;
    }

    uint8_t* base = reinterpret_cast<uint8_t*>(self->field_001c);
    uint32_t index = 0;
    for (;;) {
        uint8_t* entry = base + (size_t)index * 40;
        if (CSF_AT(float, entry, 0x1C) == position[0] &&
            CSF_AT(float, entry, 0x20) == position[1] &&
            CSF_AT(float, entry, 0x24) == position[2]) {
            return (int)index;
        }
        if (++index >= entryCount) {
            return -1;
        }
    }
}

HOOK(0x00458980, S_00458980_FindEntryIndexByPosition, "auto");
