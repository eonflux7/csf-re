#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x0078B4E0
// Returns the signed number of 164-byte entries held between the
// array begin pointer at +0x04 and the array end pointer at +0x08,
// or 0 when the begin pointer is null. The original computes this with
// a magic-number signed division by 164.
static int __fastcall DataSetPerf_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x08);
    return (end - begin) / 164;
}

HOOK(0x0078B4E0, DataSetPerf_GetEntryCount, "auto");
