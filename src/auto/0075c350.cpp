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

// Copies a range of waypoint graph nodes (11 dwords each, 0x2C bytes) from
// [srcBegin, srcEnd) to dstBegin. The first 7 dwords are copied directly;
// the trailing sub-object at +0x1C is copied through WaypointNode_CopyTail.
// Returns the one-past-last destination element, or dstBegin when the source range is empty.
CSF_FUNC(int, __fastcall, WaypointNode_CopyTail, 0x0075C090, (void* self, void* edx, int a2));

static uint32_t* __cdecl WaypointGraph_CopyNodeRange(uint32_t* srcBegin, uint32_t* srcEnd, uint32_t* dstBegin, int /*unused*/)
{
    uint32_t* src = srcBegin;
    if (src == srcEnd)
        return dstBegin;

    uint32_t* dst = dstBegin;
    do
    {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        WaypointNode_CopyTail(reinterpret_cast<void*>(dst + 7), nullptr, reinterpret_cast<int>(src + 7));
        src += 11;
        dst += 11;
    } while (src != srcEnd);

    return dst;
}

HOOK(0x0075C350, WaypointGraph_CopyNodeRange, "auto");
