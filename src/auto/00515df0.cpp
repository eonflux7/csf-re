#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Clears the byte at offset +77 of every 80-byte entry in the range [field_0008, field_000c).
// Returns the address of the last entry touched, or 0 when the range is empty.
static int __fastcall C_00840af8_ClearEntryFlags(C_00840af8* self, void* /*edx*/) {
    int result = static_cast<int>(self->field_0008);
    if (result) {
        int32_t bytesRange = static_cast<int32_t>(self->field_000c - self->field_0008);
        result = bytesRange / 80;
    }
    if (result) {
        int offsetFromBase = 0;
        int remaining = result;
        do {
            // The original reloads field_0008 on every iteration.
            result = static_cast<int>(self->field_0008) + offsetFromBase;
            offsetFromBase += 80;
            --remaining;
            *reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(result)) + 77) = 0;
        } while (remaining);
    }
    return result;
}

HOOK(0x00515DF0, C_00840af8_ClearEntryFlags, "auto");
