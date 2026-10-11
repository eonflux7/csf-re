#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies a 12-byte element (three dwords) from src into dst.
// Does nothing when dst is null. Returns dst.
static uint32_t* __stdcall CopyConstructTwelveByteElement(uint32_t* dst, uint32_t* src) {
    uint32_t* result = dst;
    if (dst) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
    }
    return result;
}

HOOK(0x004A3200, CopyConstructTwelveByteElement, "auto");
