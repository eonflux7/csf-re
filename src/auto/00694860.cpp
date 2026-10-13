#include <cstdint>
#include <cstddef>
#include "hook.h"

// Extracts the extension of `path` (text after the last '.' that follows the last
// path separator) into `outBuf`. `outCapacity` is the destination size as passed by
// the caller, compared as an unsigned pointer-sized value.
static const char* __cdecl CoreString_ExtractFileExtension(const char* path, char* outBuf, const char* outCapacity)
{
    // The original computes strlen inline (no call), so the length loop is written out.
    size_t length = 0;
    while (path[length] != '\0')
        ++length;

    if (length != 0)
    {
        for (int index = static_cast<int>(length) - 1; index != -1; --index)
        {
            char c = path[index];

            if (c == '/' || c == '\\')
            {
                // Separator before any dot: no extension.
                if (outCapacity)
                {
                    outBuf[0] = '\0';
                    return outBuf;
                }
                return outCapacity;
            }

            if (c == '.')
            {
                int afterDot = index + 1;
                size_t remaining = length - static_cast<size_t>(index);

                if (reinterpret_cast<uintptr_t>(outCapacity) < static_cast<uintptr_t>(remaining))
                {
                    // Destination too small: the original returns the computed length, unchanged.
                    return reinterpret_cast<const char*>(remaining);
                }

                // Copy the extension (including its terminator) to outBuf.
                const char* src = path + afterDot;
                const char* p = src;
                char copied;
                do
                {
                    copied = *p;
                    outBuf[p - src] = copied;
                    ++p;
                } while (copied);
                return p;
            }
        }
    }

    // No separator and no dot: empty extension.
    if (outCapacity)
    {
        outBuf[0] = '\0';
    }
    return outCapacity;
}

HOOK(0x00694860, CoreString_ExtractFileExtension, "auto");
