#include <cstdint>
#include "hook.h"

// Copies the 8-byte pair (two dwords) pointed to by src into the object
// pointed to by dest, if dest is non-null. Returns dest unchanged.
static uint32_t* __stdcall CopyTwoDwordsIfDestNonNull(uint32_t* dest, const uint32_t* src)
{
    uint32_t* result = dest;
    if (dest)
    {
        *dest = *src;
        dest[1] = src[1];
    }
    return result;
}

HOOK(0x0071A2E0, CopyTwoDwordsIfDestNonNull, "auto");
