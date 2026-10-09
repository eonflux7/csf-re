#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x00468450
// Returns the signed number of 68-byte entries held between the
// array begin pointer at +0x04 and the array end pointer at +0x08,
// or 0 when the begin pointer is null. The original computes this with
// a magic-number signed division by 68.
static int __fastcall MissionScript_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x08);
    return (end - begin) / 68;
}

HOOK(0x00468450, MissionScript_GetEntryCount, "auto");
