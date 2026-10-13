#include <cstdint>
#include "hook.h"

// Copies 12-byte records (three dwords each) from [sourceBegin, sourceEnd) into
// consecutive slots starting at destination. Returns the destination pointer
// advanced past the last record written.
static uint32_t* __cdecl TextDatabase_CopyTwelveByteRange(uint32_t* sourceBegin, uint32_t* sourceEnd, uint32_t* destination)
{
    uint32_t* source = sourceBegin;
    uint32_t* dest = destination;
    while (source != sourceEnd) {
        dest[0] = source[0];
        dest[1] = source[1];
        dest[2] = source[2];
        source += 3;
        dest += 3;
    }
    return dest;
}

HOOK(0x004EF1F0, TextDatabase_CopyTwelveByteRange, "auto");
