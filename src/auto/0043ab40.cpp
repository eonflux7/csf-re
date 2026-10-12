#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x0043AB40
// Returns the signed number of 40-byte entries held between the array
// begin pointer at +0x04 and the array end pointer at +0x08, or 0 when
// the begin pointer is null. The original divides by 40 using a signed
// magic-number sequence.
static int __fastcall Minimap_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x08);
    return (end - begin) / 40;
}

HOOK(0x0043AB40, Minimap_GetEntryCount, "auto");
