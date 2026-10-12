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

// Returns the number of 60-byte sector entries between the range's begin
// (+0x08) and end (+0x0C) pointers. An empty range (null begin) yields 0.
static int __fastcall SceneLoader_GetSectorCount(S_006d4450* self, void* /*edx*/)
{
    int beginValue = (int)self->field_0008;
    if (beginValue == 0)
        return beginValue;

    int byteSpan = (int)self->field_000c - beginValue;
    return byteSpan / 60;
}

HOOK(0x6D4190, SceneLoader_GetSectorCount, "auto");
