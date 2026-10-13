#include <cstdint>
#include <cstddef>
#include "hook.h"

// Returns the number of 16-bit code units before the terminating zero WORD.
// The original walks the buffer to the zero terminator, then converts the
// byte distance to a WORD count and subtracts the terminator.
static int __cdecl StringFormat_GetWideTextLength(uint16_t* text)
{
    uint16_t* cursor = text;
    while (*cursor++ != 0)
    {
    }
    return static_cast<int>(cursor - text - 1);
}

HOOK(0x00695560, StringFormat_GetWideTextLength, "auto");
