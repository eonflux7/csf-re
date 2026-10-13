#include <cstdint>
#include "hook.h"

// Copies 8-byte records (two dwords each) from [srcBegin, srcEnd) to dst.
// Returns the position just past the last record written.
static uint32_t* __cdecl MpRules_CopyVectorRange(uint32_t* srcBegin, uint32_t* srcEnd, uint32_t* dst) {
    uint32_t* cursor = srcBegin;
    uint32_t* out = dst;
    while (cursor != srcEnd) {
        out[0] = cursor[0];
        out[1] = cursor[1];
        cursor += 2;
        out += 2;
    }
    return out;
}

HOOK(0x00698F80, MpRules_CopyVectorRange, "auto");
