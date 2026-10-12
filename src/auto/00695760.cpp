#include <cstdint>
#include "hook.h"

// Compares two zero-terminated UTF-16 strings, returning the difference of the
// first mismatching code units, or the difference of the terminating units when
// the strings are equal up to the shorter length. Behaves like wcscmp.
static int WideString_Compare(uint16_t* a1, uint16_t* a2) {
    int result = 0;
    while (true) {
        if (*a1 == 0 || *a2 == 0) {
            break;
        }
        uint16_t c1 = *a1;
        uint16_t c2 = *a2;
        result = static_cast<int>(c1) - static_cast<int>(c2);
        ++a1;
        ++a2;
        if (c1 != c2) {
            return result;
        }
    }
    if (result == 0) {
        return static_cast<int>(*a1) - static_cast<int>(*a2);
    }
    return result;
}

HOOK(0x695760, WideString_Compare, "auto");
