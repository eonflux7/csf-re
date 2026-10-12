#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Copies 20-byte records (five dwords each) from the range ending at srcEnd
// down to lowerBound (exclusive) into the range ending at dstEnd, walking
// backwards one record at a time. Returns the destination pointer after the
// last record written (dstEnd minus the number of records copied).
static uint32_t* __cdecl CopyIconRecordsBackward(uint32_t* lowerBound,
                                                 uint32_t* srcEnd,
                                                 uint32_t* dstEnd,
                                                 int /*unused*/) {
    uint32_t* src = srcEnd;
    uint32_t* dst = dstEnd;

    while (src != lowerBound) {
        src -= 5;
        dst -= 5;
        for (int i = 0; i < 5; ++i) {
            dst[i] = src[i];
        }
    }
    return dst;
}

HOOK(0x458490, CopyIconRecordsBackward, "auto");
