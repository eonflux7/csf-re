#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Returns true when any 80-byte entry in [field_0008, field_000c) has a
// nonzero flag byte at offset +0x4D.
static char __fastcall C_00840af8_HasOccupiedSlot(C_00840af8* self, void* /*edx*/) {
    uint32_t entryCount = self->field_0008;
    if (entryCount) {
        entryCount = static_cast<uint32_t>(
            static_cast<int>(self->field_000c - self->field_0008) / 80);
    }

    uint32_t index = 0;
    if (entryCount == 0) {
        return 0;
    }

    uint8_t* flagByte = reinterpret_cast<uint8_t*>(self->field_0008 + 0x4D);
    while (*flagByte == 0) {
        if (++index >= entryCount) {
            return 0;
        }
        flagByte += 80;
    }
    return 1;
}

HOOK(0x515DA0, C_00840af8_HasOccupiedSlot, "auto");
