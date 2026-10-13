#include <cstdint>
#include "globals.h"
#include "hook.h"

// Walks backward from srcEnd down to stopAddr in 24-byte records, copying the
// dwords at +4, +8, +16 and +20 from each source record into the matching
// destination record. Dwords +0 and +12 of the destination are left untouched.
// Returns the destination cursor after the last record (dstEnd - 24 * count).
static int __cdecl SceneRecords_CopyFieldsBackward(int stopAddr, int srcEnd, int dstEnd, int /*unused*/) {
    int result = dstEnd;
    int srcCur = srcEnd;
    if (stopAddr != srcCur) {
        do {
            srcCur -= 24;
            result -= 24;
            uint8_t* src = reinterpret_cast<uint8_t*>(srcCur);
            uint8_t* dst = reinterpret_cast<uint8_t*>(result);
            CSF_AT(uint32_t, dst, 0x04) = CSF_AT(uint32_t, src, 0x04);
            CSF_AT(uint32_t, dst, 0x08) = CSF_AT(uint32_t, src, 0x08);
            CSF_AT(uint32_t, dst, 0x10) = CSF_AT(uint32_t, src, 0x10);
            CSF_AT(uint32_t, dst, 0x14) = CSF_AT(uint32_t, src, 0x14);
        } while (srcCur != stopAddr);
    }
    return result;
}

HOOK(0x44FA50, SceneRecords_CopyFieldsBackward, "auto");
