#include <cstdint>
#include "hook.h"

// Copies the bit range [srcWord:srcBit, srcEndWord:srcEndBit) from a source bitmap into a
// destination bitmap starting at dstWord:dstBit, one bit at a time. Each bit is set or cleared
// in the destination. Writes the final destination cursor (bit index, word pointer) into
// cursorOut and returns cursorOut.
static uint32_t* __cdecl MissionScript_CopyBitRange(
    uint32_t* cursorOut,
    uint32_t srcBit,
    uint32_t* srcWord,
    uint32_t srcEndBit,
    uint32_t* srcEndWord,
    uint32_t dstBit,
    uint32_t* dstWord)
{
    uint32_t curDstBit = dstBit;

    while (srcWord != srcEndWord || srcBit != srcEndBit) {
        if (((1u << srcBit) & *srcWord) != 0) {
            *dstWord = *dstWord | (1u << curDstBit);
        } else {
            *dstWord = *dstWord & ~(1u << curDstBit);
        }

        if (dstBit >= 0x1F) {
            curDstBit = 0;
            ++dstWord;
        } else {
            curDstBit = dstBit + 1;
        }
        dstBit = curDstBit;

        if (srcBit >= 0x1F) {
            srcBit = 0;
            ++srcWord;
        } else {
            ++srcBit;
        }
    }

    cursorOut[0] = curDstBit;
    cursorOut[1] = (uint32_t)(uintptr_t)dstWord;
    return cursorOut;
}

HOOK(0x0046F010, MissionScript_CopyBitRange, "auto");
