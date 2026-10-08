#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x0042E1E0
// Returns the signed number of 48-byte entries held between the
// array begin pointer at +0x04 and the array end pointer at +0x08,
// or 0 when the begin pointer is null. The original computes this with
// the same magic-number signed division by 48.
static int __fastcall HudList_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x08);
    return (end - begin) / 48;
}

HOOK(0x0042E1E0, HudList_GetEntryCount, "auto");
