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

// Copies a 16-byte embedded subobject (at record +0x1C) from source to destination record.
CSF_FUNC(int, __fastcall, WaypointGraph_CopyEmbeddedSubobject, 0x0075C090, (void* self, void* edx, int a2));

// Copies a range of 44-byte waypoint graph records backwards from `last` down to `first`,
// writing them ending at `destEnd`. Returns the final destination cursor (the start of the copied block).
static uint32_t* __cdecl WaypointGraph_CopyRecordsBackward(uint32_t* first, uint32_t* last, uint32_t* destEnd, int /*unusedArg*/)
{
    uint32_t* src = last;
    if (first == last)
        return destEnd;

    uint32_t* dst = destEnd;
    do
    {
        src -= 11;
        dst -= 11;

        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];

        WaypointGraph_CopyEmbeddedSubobject(
            reinterpret_cast<void*>(dst + 7),
            nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(src + 7)));
    }
    while (src != first);

    return dst;
}

HOOK(0x0075C2C0, WaypointGraph_CopyRecordsBackward, "auto");
