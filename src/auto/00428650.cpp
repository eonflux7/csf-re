#include <cstdint>
#include "hook.h"

// Scans a table of 93 five-dword entries starting at this+4 for an entry whose
// first two dwords equal (a2, a3). Returns the zero-based entry index, or -1.
static int __fastcall Cheats_FindEntryIndex(uint32_t* self, void* /*edx*/, int a2, int a3) {
    int index = 0;
    uint32_t* entry = self + 1;
    const uint32_t wantedFirst = static_cast<uint32_t>(a2);
    const uint32_t wantedSecond = static_cast<uint32_t>(a3);

    while (entry[0] != wantedFirst || entry[1] != wantedSecond) {
        entry += 5;
        ++index;
        if (index >= 0x5D) {
            return -1;
        }
    }
    return index;
}

HOOK(0x428650, Cheats_FindEntryIndex, "auto");
