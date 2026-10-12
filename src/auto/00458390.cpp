#include <cstdint>
#include "hook.h"

// Fills the 5-dword records in [first, last) (stride 0x14) with the 5 dwords at `source`.
// Returns the end cursor, which equals `last` when the loop terminates normally.
static uint32_t* HudStatusIcons_FillRecordRange(uint32_t* first, uint32_t* last, const uint32_t* source) {
    uint32_t* cursor = first;
    while (cursor != last) {
        uint32_t* record = cursor;
        record[0] = source[0];
        record[1] = source[1];
        record[2] = source[2];
        record[3] = source[3];
        cursor += 5;
        record[4] = source[4];
    }
    return cursor;
}

HOOK(0x458390, HudStatusIcons_FillRecordRange, "auto");
