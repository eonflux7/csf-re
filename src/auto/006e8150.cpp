// gfx_hanim: copies `count` 36-byte keyframe records (9 dwords each) from the
// source HAnim's record array, starting at index a3, into the destination's array
// from index 0. Returns the number of records copied.
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

static int __cdecl HAnim_CopyKeyframeRange(int dstHAnim, int srcHAnim, int srcStartIndex)
{
    uint8_t* dstObj = reinterpret_cast<uint8_t*>(dstHAnim);
    uint8_t* srcObj = reinterpret_cast<uint8_t*>(srcHAnim);

    int recordCount = CSF_AT(int, dstObj, 0x4);
    int copied = 0;
    for (; copied < recordCount; ++copied)
    {
        uint8_t* srcArray = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, srcObj, 0x20));
        int srcStride = CSF_AT(int, srcArray, 0x24);
        uint8_t* srcRecord = srcArray
            + static_cast<ptrdiff_t>(srcStride) * static_cast<ptrdiff_t>(copied + srcStartIndex)
            + 76;

        uint8_t* dstArray = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, dstObj, 0x20));
        int dstStride = CSF_AT(int, dstArray, 0x24);
        uint8_t* dstRecord = dstArray
            + static_cast<ptrdiff_t>(dstStride) * static_cast<ptrdiff_t>(copied)
            + 76;

        std::memcpy(dstRecord, srcRecord, 0x24);
    }
    return copied;
}

HOOK(0x006E8150, HAnim_CopyKeyframeRange, "auto");
