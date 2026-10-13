#include <cstdint>
#include <cstddef>
#include "hook.h"

// Copies a zero-terminated UTF-16 string into a byte buffer, keeping the low byte
// of each code unit, and writes at most maxChars characters. Returns the count copied.
static unsigned int __cdecl StringFormat_ConvertWideTextToNarrow(const uint16_t* wideText,
                                                                 uint8_t* narrowOut,
                                                                 unsigned int maxChars)
{
    const uint16_t* cursor = wideText;
    unsigned int count = 0;

    if (*wideText != 0)
    {
        do
        {
            if (count >= maxChars)
                break;
            narrowOut[count] = static_cast<uint8_t>(*cursor);
            cursor++;
            ++count;
        } while (*cursor != 0);

        narrowOut[count] = 0;
    }
    else
    {
        narrowOut[0] = 0;
    }

    return count;
}

HOOK(0x695620, StringFormat_ConvertWideTextToNarrow, "auto");
