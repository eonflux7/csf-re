#include <cstdint>
#include "hook.h"

// Copies a narrow, NUL-terminated string into a 16-bit buffer, widening each
// byte with sign extension. Writes at most maxCount-1 characters plus a
// terminating NUL. Returns the number of characters copied.
static unsigned int __cdecl StringFormat_CopyTrucoStringToWide(char* src, uint16_t* dst, int maxCount)
{
    const char* cursor = src;
    char current = *src;
    unsigned int copied = 0;

    if (current != 0)
    {
        do
        {
            if (copied >= static_cast<unsigned int>(maxCount - 1))
                break;

            dst[copied] = static_cast<uint16_t>(static_cast<int16_t>(current));
            current = cursor[1];
            ++copied;
            ++cursor;
        } while (current != 0);

        dst[copied] = 0;
    }
    else
    {
        *dst = 0;
    }

    return copied;
}

HOOK(0x00695590, StringFormat_CopyTrucoStringToWide, "auto");
