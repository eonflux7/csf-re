#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original function: 0x004F6E90
// Scans a table of 48-byte entries stored at this+0x11C (count at this+0x124).
// When entry[0] equals a2, copies selected dwords of that entry into the caller's
// output buffer a3 (indices 0, 2, 3, 5..11; index 1 and 4 are left untouched) and returns 1.
// Returns 0 if no entry matches.
static char __fastcall ObjectProvider_TryFindIndexedEntry(S_004f6e90* self, void* /*edx*/, int a2, uint32_t* a3)
{
    char result = 0;
    uint32_t entryOffset = 0;
    uint32_t entryIndex = 0;

    while (entryIndex < CSF_AT(uint32_t, self, 0x124))
    {
        uint8_t* entryTable = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x11C));
        uint32_t* entry = reinterpret_cast<uint32_t*>(entryTable + entryOffset);

        if (entry[0] == static_cast<uint32_t>(a2))
        {
            a3[0] = entry[0];
            a3[2] = entry[2];
            a3[3] = entry[3];
            a3[5] = entry[5];
            a3[6] = entry[6];
            a3[7] = entry[7];
            a3[8] = entry[8];
            a3[9] = entry[9];
            a3[10] = entry[10];
            a3[11] = entry[11];
            result = 1;
        }

        entryOffset += 0x30;
        ++entryIndex;

        if (result)
            break;
    }

    return result;
}

HOOK(0x004F6E90, ObjectProvider_TryFindIndexedEntry, "auto");
