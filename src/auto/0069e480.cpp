#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Classifies the byte at the tokenizer's current position (buffer base + index).
// Returns 1 for a quote, 0 for control/space, 3 for identifier/number chars and '-', 2 for other.
static int __fastcall S_0069e480_ClassifyCurrentChar(S_0069e480* self, void* /*edx*/)
{
    const uint8_t currentChar =
        *(reinterpret_cast<uint8_t*>(self->field_0018) + self->field_0008);

    if (currentChar == 34)
        return 1;
    if (currentChar <= 0x20u)
        return 0;
    if (currentChar >= 0x30u && currentChar <= 0x39u)
        return 3;
    if (currentChar >= 0x61u && currentChar <= 0x7Au)
        return 3;
    if (currentChar >= 0x41u && currentChar <= 0x5Au)
        return 3;
    if (currentChar == 95)
        return 3;
    if (currentChar == 46)
        return 3;
    if (currentChar == 45)
        return 3;
    return 2;
}

HOOK(0x69E480, S_0069e480_ClassifyCurrentChar, "auto");
