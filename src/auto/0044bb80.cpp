#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x0044BB80
// Returns the signed number of 100-byte entries held between the
// array begin pointer at +0x10 and the array end pointer at +0x14,
// or 0 when the begin pointer is null. The original computes this with
// the same magic-number signed division by 100.
static int __fastcall ListEntries_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x10);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x14);
    const int32_t byteSpan = static_cast<int32_t>(static_cast<uint32_t>(end) - static_cast<uint32_t>(begin));
    return byteSpan / 100;
}

HOOK(0x0044BB80, ListEntries_GetEntryCount, "auto");
