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

// Returns the number of 56-byte entries between the range's begin (+0x04)
// and end (+0x08) pointers. An empty range (null begin) yields 0.
static int __fastcall Campaign_GetRangeEntryCount(void* self, void* /*edx*/)
{
    uint32_t begin = CSF_AT(uint32_t, self, 0x4);
    if (begin == 0)
        return 0;

    uint32_t end = CSF_AT(uint32_t, self, 0x8);
    int byteSpan = (int)(end - begin);
    return byteSpan / 56;
}

HOOK(0x43E040, Campaign_GetRangeEntryCount, "auto");
