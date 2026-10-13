#include <cstdint>
#include <cstddef>
#include "hook.h"

// Ordering predicate for type-name strings: true when a1 sorts strictly before a2.
// Characters are compared as unsigned bytes; the first mismatch decides the result.
static bool __stdcall TypeRegistry_IsStringLess(const char* a1, const char* a2)
{
    const uint8_t* left = reinterpret_cast<const uint8_t*>(a1);
    const uint8_t* right = reinterpret_cast<const uint8_t*>(a2);

    for (;;) {
        uint8_t leftChar = *left;
        uint8_t rightChar = *right;
        if (leftChar != rightChar) {
            return leftChar < rightChar;
        }
        if (leftChar == 0) {
            return false; // both strings ended together: equal, not less
        }
        ++left;
        ++right;
    }
}

HOOK(0x0054ABF0, TypeRegistry_IsStringLess, "auto");
