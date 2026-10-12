#include <cstdint>
#include "hook.h"

// Returns the index of the highest set bit of value (floor(log2(value))),
// or -1 when value is 0.
static int __cdecl HighestSetBitIndex(unsigned int value) {
    if (value == 0) {
        return -1;
    }
    unsigned int shifted = value >> 1;
    int result = 0;
    for (; shifted != 0; ++result) {
        shifted >>= 1;
    }
    return result;
}

HOOK(0x0069B730, HighestSetBitIndex, "auto");
