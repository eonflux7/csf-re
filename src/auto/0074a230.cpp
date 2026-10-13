#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Sets the secondary-state byte (offset +47) of spawn-point record `index`
// (48-byte records, 30 entries) to 1. Returns the record's byte offset, or
// the input index unchanged when it is out of range. Signed compares, as in
// the original's jl / jge.
static int __fastcall MpSpawnPoints_UpdateSecondaryState(void* self, void* /*edx*/, int index)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    if (index >= 0 && index < 30)
    {
        int recordOffset = index * 48;
        CSF_AT(uint8_t, base, recordOffset + 47) = 1;
        return recordOffset;
    }
    return index;
}

HOOK(0x74A230, MpSpawnPoints_UpdateSecondaryState, "auto");
