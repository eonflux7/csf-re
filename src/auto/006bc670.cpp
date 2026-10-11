#include <cstdint>
#include "hook.h"

// Copies consecutive 24-byte records (six dwords each) from `source` into the
// range [first, last), stepping the destination by one record at a time. The
// source pointer is not advanced, so every destination record receives the same
// six dwords. Returns the destination pointer where the loop stopped (equal to
// `last` when `first` is aligned to the record stride).
static uint32_t* __cdecl CopyRecordRangeSixDwords(uint32_t* first, uint32_t* last, uint32_t* source)
{
    uint32_t* cursor = first;
    while (cursor != last) {
        uint32_t* record = cursor;
        record[0] = source[0];
        record[1] = source[1];
        record[2] = source[2];
        record[3] = source[3];
        record[4] = source[4];
        cursor += 6;
        record[5] = source[5];
    }
    return cursor;
}

HOOK(0x006BC670, CopyRecordRangeSixDwords, "auto");
