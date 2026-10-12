#include <cstdint>
#include "globals.h"
#include "hook.h"

// Scans a table of 16-byte entries (count at +0x08, base address at +0x0C) for the
// entry whose address equals `address`. Returns the base address when the match is
// the last entry, the address of the following entry otherwise, or 0 if no match.
static int __fastcall FindNextEntryAfter(void* self, void* /*edx*/, int address) {
    int result = 0;
    uint32_t index = 0;
    int offset = 0;
    do {
        if (index >= CSF_AT(uint32_t, self, 0x08))
            break;
        int entryBase = CSF_AT(int, self, 0x0C);
        if (address == offset + entryBase) {
            result = CSF_AT(int, self, 0x0C);
            if (index != CSF_AT(uint32_t, self, 0x08) - 1)
                result = offset + entryBase + 0x10;
        }
        ++index;
        offset += 0x10;
    } while (!result);
    return result;
}

HOOK(0x4E0830, FindNextEntryAfter, "auto");
