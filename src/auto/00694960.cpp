#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Copies a path into a caller buffer, truncated to its directory part (up to and
// including the last '/' or '\'). Returns the original's pointer-typed result,
// which is an integer offset or capacity in some paths, exactly as the original does.
static const char* __cdecl CoreString_ExtractResourceDirectory(const char* a1, uint8_t* a2, uint32_t a3)
{
    // Inline length computation (the original does not call strlen).
    uint32_t len = 0;
    while (a1[len] != 0)
        ++len;

    uint32_t idx = len - 1;   // wraps to 0xFFFFFFFF for an empty string
    bool found = false;

    // Scan backwards for the last path separator.
    if (len != 0)
    {
        for (;;)
        {
            char c = a1[idx];
            if (c == '/' || c == '\\')
            {
                found = true;
                break;
            }
            if (--idx == 0xFFFFFFFFu)
                break;
        }
    }

    if (found)
    {
        const char* result = reinterpret_cast<const char*>(static_cast<uintptr_t>(idx + 1));

        if (idx + 1 < a3)
        {
            // Copy the whole source string (including its terminator) into a2.
            const char* src = a1;
            char c;
            do
            {
                c = *src;
                a2[src - a1] = static_cast<uint8_t>(c);
                ++src;
            } while (c);

            result = src;
            a2[idx + 1] = 0;
        }
        return result;
    }

    // No separator found (or empty string): return a3 and clear the buffer's first byte.
    const char* result = reinterpret_cast<const char*>(static_cast<uintptr_t>(a3));
    if (a3 != 0)
        a2[0] = 0;
    return result;
}

HOOK(0x694960, CoreString_ExtractResourceDirectory, "auto");
