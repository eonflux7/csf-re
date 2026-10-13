#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Writes the source's +4/+8 dwords into entry[entryIndex] of the table at
// this->field_0198 (28-byte entries), then sets bit 1 of that entry's +24 flags.
// Returns a pointer to the flags dword.
static uint32_t* __fastcall C_008342e8_PrepareEntryFrame(C_008342e8* self, void* /*edx*/, int entryIndex, int sourceAddr)
{
    uint8_t* source = reinterpret_cast<uint8_t*>(sourceAddr);

    uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0198 + 28 * entryIndex);
    CSF_AT(uint32_t, entry, 4) = CSF_AT(uint32_t, source, 4);
    CSF_AT(uint32_t, entry, 8) = CSF_AT(uint32_t, source, 8);

    uint8_t* flagsEntry = reinterpret_cast<uint8_t*>(self->field_0198 + 28 * entryIndex);
    uint32_t* flags = &CSF_AT(uint32_t, flagsEntry, 24);
    *flags |= 2u;
    return flags;
}

HOOK(0x0049EDC0, C_008342e8_PrepareEntryFrame, "auto");
