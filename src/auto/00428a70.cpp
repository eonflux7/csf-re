#include <cstdint>
#include "hook.h"

// Fills `count` 20-byte records starting at `table`. Each record is:
//   +0  int32  -1
//   +4  int32  -1
//   +8  uint32 0
//   +12 float  1.0f
//   +16 uint32 0
// Returns the pointer just past the last record written (or `table` unchanged
// when table is null or count is zero).
static uint32_t* __cdecl Cheats_InitEntryTable(uint32_t* table, int count) {
    uint32_t* cursor = table;
    if (table != nullptr) {
        int remaining = count;
        if (remaining != 0) {
            cursor = table + 2;
            do {
                // Pseudocode writes relative to the advanced cursor (cursor - 2 .. cursor + 2).
                cursor[-2] = 0xFFFFFFFFu;
                cursor[-1] = 0xFFFFFFFFu;
                cursor[0] = 0;
                *reinterpret_cast<float*>(&cursor[1]) = 1.0f;
                cursor[2] = 0;
                cursor += 5;
                --remaining;
            } while (remaining != 0);
        }
    }
    return cursor;
}

HOOK(0x428A70, Cheats_InitEntryTable, "auto");
