#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Fills the 12-byte records in [rangeBegin, rangeEnd) with the 3-dword value
// at `source`, re-reading `source` for every record. Returns the end cursor.
static uint32_t* __cdecl FillDwordTripleRange(uint32_t* rangeBegin, uint32_t* rangeEnd, uint32_t* source) {
    uint32_t* cursor = rangeBegin;
    while (cursor != rangeEnd) {
        uint32_t* record = cursor;
        record[0] = source[0];
        record[1] = source[1];
        cursor += 3;
        record[2] = source[2];
    }
    return cursor;
}

HOOK(0x0044AD30, FillDwordTripleRange, "auto");
