#include <cstdint>
#include "hook.h"

// Fills the dword range [begin, end) with the value currently at *source.
// *source is re-read on every iteration, as in the original loop.
// Returns the cursor where the loop stopped: `end` when begin != end, otherwise `begin`.
static uint32_t* __cdecl MpSpawnPoints_FillDwordRange(uint32_t* begin, uint32_t* end, uint32_t* source) {
    uint32_t* cursor = begin;
    while (cursor != end) {
        *cursor = *source;
        ++cursor;
    }
    return cursor;
}

HOOK(0x551CD0, MpSpawnPoints_FillDwordRange, "auto");
