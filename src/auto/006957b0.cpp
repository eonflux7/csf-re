#include <cstdint>
#include "hook.h"

static int __cdecl CoreVariantValues_CompareWideStringsIgnoreAsciiCase(
    uint16_t* left, uint16_t* right)
{
    for (;;)
    {
        const uint16_t leftChar = *left;
        if (!leftChar)
            break;

        const uint16_t rightChar = *right;
        if (!rightChar)
            break;

        const int foldedLeft =
            (leftChar < 0x41u || leftChar > 0x5Au) ? leftChar : leftChar + 32;
        const int foldedRight =
            (rightChar < 0x41u || rightChar > 0x5Au) ? rightChar : rightChar + 32;
        const int difference = foldedLeft - foldedRight;

        ++left;
        ++right;
        if (difference)
            return difference;
    }

    const uint16_t leftChar = *left;
    const int foldedLeft =
        (leftChar < 0x41u || leftChar > 0x5Au) ? leftChar : leftChar + 32;

    const uint16_t rightChar = *right;
    const int foldedRight =
        (rightChar < 0x41u || rightChar > 0x5Au) ? rightChar : rightChar + 32;

    return foldedLeft - foldedRight;
}

HOOK(0x006957B0, CoreVariantValues_CompareWideStringsIgnoreAsciiCase, "auto");
