#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x00708DC0
// Returns the signed number of 44-byte entries held between the
// array begin pointer at +0x54 and the array end pointer at +0x58,
// or 0 when the begin pointer is null.
static int __fastcall C_008629a4_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x54);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x58);
    return (end - begin) / 44;
}

HOOK(0x00708DC0, C_008629a4_GetEntryCount, "auto");
