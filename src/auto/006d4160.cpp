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

// Returns the number of 60-byte records between the dictionary's begin (+0x04)
// and end (+0x08) pointers. An empty dictionary (null begin) yields 0.
static int __fastcall ClumpDict_GetRecordCount(S_006d4160* self, void* /*edx*/)
{
    uint32_t begin = self->field_0004;
    if (begin == 0)
        return (int)begin;

    uint32_t end = self->field_0008;
    int byteSpan = (int)(end - begin);
    return byteSpan / 60;
}

HOOK(0x6D4160, ClumpDict_GetRecordCount, "auto");
