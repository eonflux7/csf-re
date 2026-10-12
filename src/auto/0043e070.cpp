#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x0043E070
// Returns the signed number of 196-byte entries held between the
// array begin pointer at +0x04 and the array end pointer at +0x08,
// or 0 when the begin pointer is null. The original computes this with
// the same magic-number signed division by 196.
static int __fastcall CampaignEntries_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x08);
    const int32_t diff = static_cast<int32_t>(static_cast<uint32_t>(end) - static_cast<uint32_t>(begin));
    return diff / 196;
}

HOOK(0x0043E070, CampaignEntries_GetEntryCount, "auto");
