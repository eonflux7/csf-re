#include <cstdint>
#include <cstddef>
#include <cstring>
#include "hook.h"

// Copies a null-terminated UTF-16 string (including its terminator) from src to dest.
// Returns the terminator word (0), or 0 when either pointer is null.
static int16_t __cdecl CoreVariantValues_CopyWideString(char* dest, char* src)
{
    int16_t result = 0;
    if (dest && src)
    {
        char* end = src;
        do
        {
            result = *reinterpret_cast<int16_t*>(end);
            end += 2;
        } while (result);

        std::memcpy(dest, src, static_cast<size_t>(end - src));
    }
    return result;
}

HOOK(0x695680, CoreVariantValues_CopyWideString, "auto");
