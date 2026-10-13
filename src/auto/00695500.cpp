#include <cstdint>
#include "hook.h"

// Compares a NUL-terminated wide (16-bit) string against a NUL-terminated narrow string,
// returning the difference of the first mismatching code units, or -1/0/1 for the tail case.
static int __cdecl StringFormat_CompareWideAgainstNarrow(uint16_t* a1, char* a2)
{
    int result = 0;
    for (;;) {
        uint16_t wideChar = *a1;
        if (wideChar == 0 || *a2 == 0)
            break;
        char narrowChar = *a2;
        result = wideChar - *a2;
        ++a1;
        ++a2;
        if (wideChar != narrowChar)
            return result;
    }

    if (result == 0) {
        int wideValue = *a1;
        int narrowValue = *a2;
        if (wideValue <= narrowValue) {
            if (wideValue < narrowValue)
                return -1;
        } else {
            return 1;
        }
    }
    return result;
}

HOOK(0x695500, StringFormat_CompareWideAgainstNarrow, "auto");
