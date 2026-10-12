#include <cstdint>
#include "hook.h"

// Appends the wide string `src` to the end of the wide string `dest`, then writes the terminator.
// Does nothing if either pointer is null. An empty `src` only writes the terminator at the end of `dest`.
static void __cdecl VariantValues_AppendWideString(uint16_t* dest, const uint16_t* src) {
    if (dest && src) {
        // Walk to the terminator of dest; `end` ends up one past the null.
        const uint16_t* end = dest;
        while (*end++) {
        }
        int destLength = (int)(end - dest) - 1;

        int copiedCount = 0;
        uint16_t ch = src[0];
        if (ch) {
            uint16_t* out = &dest[destLength];
            do {
                *out = ch;
                ch = src[++copiedCount];
                ++out;
            } while (ch);
        }
        dest[copiedCount + destLength] = 0;
    }
}

HOOK(0x695700, VariantValues_AppendWideString, "auto");
