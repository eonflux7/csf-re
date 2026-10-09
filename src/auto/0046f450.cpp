#include <cstdint>
#include "hook.h"

// Copies a bit range from the source cursor (srcWord, srcBit) backward toward the
// stop cursor (stopWord, stopBit), writing one bit at a time into the destination
// cursor (dstWord, dstBit). Each destination bit is set or cleared to match the
// source bit. Afterwards outCursor receives the final destination cursor as
// {bitIndex, wordPointer} and is returned.
static int* __cdecl MissionScript_CopyBitRangeBackward(int* outCursor, int stopBit, uint32_t* stopWord,
                                                       int srcBit, uint32_t* srcWord,
                                                       int dstBit, uint32_t* dstWord, int unusedArg) {
    (void)unusedArg;

    while (true) {
        uint32_t* srcCurrentWord = srcWord;
        if (srcWord == stopWord && srcBit == stopBit) {
            break;
        }

        if (srcBit != 0) {
            --srcBit;
        } else {
            srcCurrentWord = srcWord - 1;
            srcBit = 31;
            --srcWord;
        }

        if (dstBit != 0) {
            --dstBit;
        } else {
            dstBit = 31;
            --dstWord;
        }

        if ((*srcCurrentWord & (1u << srcBit)) != 0) {
            *dstWord |= (1u << dstBit);
        } else {
            *dstWord &= ~(1u << dstBit);
        }
    }

    outCursor[0] = dstBit;
    outCursor[1] = static_cast<int>(reinterpret_cast<uintptr_t>(dstWord));
    return outCursor;
}

HOOK(0x0046F450, MissionScript_CopyBitRangeBackward, "auto");
