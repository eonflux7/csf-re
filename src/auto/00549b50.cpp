#include <cstdint>
#include "hook.h"

// Copies the block of 24-byte records [rangeStart, rangeEnd) backward so that it
// ends at destEnd. Records are six dwords each; copying runs from the last record
// toward rangeStart. Returns the new start of the destination block.
static uint32_t* __cdecl MenuVideo_CopyRecordsBackward(uint32_t* rangeStart,
                                                       uint32_t* rangeEnd,
                                                       uint32_t* destEnd) {
    uint32_t* src = rangeEnd;
    uint32_t* dst = destEnd;

    if (src == rangeStart) {
        return dst;
    }

    do {
        src -= 6;
        dst -= 6;
        for (int i = 0; i < 6; ++i) {
            dst[i] = src[i];
        }
    } while (src != rangeStart);

    return dst;
}

HOOK(0x549B50, MenuVideo_CopyRecordsBackward, "auto");
