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

// Returns the number of 28-byte entries between the range's begin (field_0004)
// and end (field_0008) pointers. An empty range (null begin) yields 0.
static int __fastcall MissionScript_GetEntryCount(S_0046bea0* self, void* /*edx*/)
{
    uint32_t begin = self->field_0004;
    if (begin == 0)
        return 0;

    int byteSpan = (int)(self->field_0008 - begin);
    return byteSpan / 28;
}

HOOK(0x46BEA0, MissionScript_GetEntryCount, "auto");
