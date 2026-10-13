#include <cstddef>
#include <cstdint>

#include "hook.h"

// Lower-bound binary search over a sorted array of 16-byte records whose first
// dword is an unsigned key. Returns the first record whose key is >= wantedKey,
// or the record after it when every key is smaller.
static char* __stdcall Memory_LowerBoundByKey(char* rangeFirst, char* rangeLast, uint32_t wantedKey)
{
    char* low = rangeFirst;
    char* high = rangeLast;

    while (low != high)
    {
        // (high - low) >> 4 is an arithmetic shift of a signed difference.
        int32_t byteSpan = static_cast<int32_t>(high - low);
        int32_t recordCount = byteSpan >> 4;
        // cdq / sub / sar 1: halve toward zero for negative counts.
        int32_t halfCount = (recordCount + (recordCount < 0 ? 1 : 0)) >> 1;
        int32_t middleOffset = halfCount << 4;
        char* middle = low + middleOffset;
        uint32_t middleKey = *reinterpret_cast<const uint32_t*>(middle);

        if (middleKey >= wantedKey)
            high = middle;
        else
            low = middle + 16;
    }

    char* following = low + 16;
    if (*reinterpret_cast<const uint32_t*>(low) < wantedKey)
        return following;
    return low;
}

HOOK(0x00691620, Memory_LowerBoundByKey, "auto");
