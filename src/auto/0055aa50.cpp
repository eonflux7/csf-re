#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Fills consecutive 16-byte records in [rangeBegin, rangeEnd) with the four
// dwords at `source`, re-reading the source on every record. Returns the
// cursor where the loop stopped (rangeEnd when the range is walked exactly).
static uint32_t* __cdecl MpRules_FillRecordRange(uint32_t* rangeBegin,
                                                 uint32_t* rangeEnd,
                                                 uint32_t* source) {
    uint32_t* cursor = rangeBegin;
    while (cursor != rangeEnd) {
        uint32_t* record = cursor;
        record[0] = source[0];
        record[1] = source[1];
        record[2] = source[2];
        cursor += 4;
        record[3] = source[3];
    }
    return cursor;
}

HOOK(0x0055AA50, MpRules_FillRecordRange, "auto");
