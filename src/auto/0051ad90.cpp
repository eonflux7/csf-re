#include <cstdint>
#include "hook.h"

static uint8_t* __cdecl Tables_FillRangeFromSource(uint8_t* begin, uint8_t* end, uint8_t* source) {
    uint8_t* cursor = begin;
    while (cursor != end) {
        *cursor = *source;
        ++cursor;
    }
    return cursor;
}

HOOK(0x51AD90, Tables_FillRangeFromSource, "auto");
