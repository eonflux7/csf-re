#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies 16-byte blocks (4 dwords) backward from srcEnd down to stopPtr,
// writing them ending at dstEnd. Returns the final destination pointer.
static uint32_t* __cdecl CopyDwordBlocksBackward(uint32_t* stopPtr, uint32_t* srcEnd, uint32_t* dstEnd, int /*unused*/) {
    uint32_t* src = srcEnd;
    uint32_t* dst = dstEnd;

    if (src == stopPtr) {
        return dst;
    }

    do {
        src -= 4;
        dst -= 4;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    } while (src != stopPtr);

    return dst;
}

HOOK(0x6D9E40, CopyDwordBlocksBackward, "auto");
