#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Clears the bit that represents `position` in a bitmap table and records the
// affected byte index. Returns 1 when the position was in range and cleared, else 0.
static char __fastcall S_0069b1c0_ClearPositionBit(S_0069b1c0* self, void* /*edx*/, uint32_t position) {
    if (!self->field_0000)
        return 0;

    uint32_t bitmapBase = self->field_0004;
    if (position < bitmapBase || position >= self->field_000c)
        return 0;

    uint32_t bitIndex = (position - self->field_0008) / self->field_0010;
    uint32_t byteIndex = bitIndex >> 3;
    uint8_t* bitmapBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(bitmapBase));
    uint8_t clearMask = static_cast<uint8_t>(~(1u << (bitIndex & 7)));

    bitmapBytes[byteIndex] = static_cast<uint8_t>(bitmapBytes[byteIndex] & clearMask);
    --self->field_001c;
    self->field_0018 = byteIndex;
    return 1;
}

HOOK(0x0069B1C0, S_0069b1c0_ClearPositionBit, "auto");
