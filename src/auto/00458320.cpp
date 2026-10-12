#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Fills a range of 40-byte status-icon entries [begin, end). Each entry gets
// the first 0x1C bytes of src copied in, then src[8] at entry+0x20 and
// src[9] at entry+0x24. Returns the end pointer reached.
static char* __cdecl FillStatusIconRange(char* begin, char* end, uint32_t* src) {
    char* cursor = begin;
    if (cursor != end) {
        do {
            std::memcpy(cursor, src, 0x1C);
            CSF_AT(uint32_t, cursor, 0x20) = CSF_AT(uint32_t, src, 0x20);
            CSF_AT(uint32_t, cursor, 0x24) = CSF_AT(uint32_t, src, 0x24);
            cursor += 40;
        } while (cursor != end);
    }
    return cursor;
}

HOOK(0x458320, FillStatusIconRange, "auto");
