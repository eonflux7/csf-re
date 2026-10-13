#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Scans a table of 28-byte entries (table pointer at +0, entry count at +8) for the
// first entry whose dword at +0 is nonzero and whose 3 floats at entry+0x10/+0x14/+0x18
// equal the given 3-float key. Returns that entry's index, or -1 if the table is empty
// or nothing matches.
static int __fastcall FindEntryIndexByVector3(uint32_t* self, void* /*edx*/, float* key) {
    uint32_t entryCount = self[2];
    uint32_t index = 0;
    if (entryCount == 0) {
        return -1;
    }
    uint32_t* entry = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self[0]));
    float* field = reinterpret_cast<float*>(entry + 6);
    while (*entry == 0 || field[-2] != key[0] || field[-1] != key[1] || field[0] != key[2]) {
        ++index;
        entry += 7;
        field += 7;
        if (index >= entryCount) {
            return -1;
        }
    }
    return static_cast<int>(index);
}

HOOK(0x004DC5F0, FindEntryIndexByVector3, "auto");
