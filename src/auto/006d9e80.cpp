#include <cstdint>
#include "hook.h"

// Copies a 16-byte element (four dwords) from src to dst. Does nothing when dst is null.
// Returns dst unchanged.
static uint32_t* __stdcall CopySixteenByteElement(uint32_t* dst, uint32_t* src) {
    uint32_t* result = dst;
    if (dst) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    }
    return result;
}

HOOK(0x6D9E80, CopySixteenByteElement, "auto");
