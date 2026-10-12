#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Walks the 16-byte entries of a ring (count at +0x8, base at +0xC) looking for
// the entry whose address equals a2. Returns the entry before it, or the last
// entry when the match is the first one. Returns 0 if no entry matches.
static int __fastcall CircularList_PrevOf(void* self, void* /*edx*/, int a2) {
    uint32_t count = CSF_AT(uint32_t, self, 0x8);
    uint32_t result = 0;
    uint32_t index = 0;
    do {
        if (index >= count)
            break;
        uint32_t base = CSF_AT(uint32_t, self, 0xC);
        uint32_t entry = base + index * 16;
        if (static_cast<uint32_t>(a2) == entry) {
            if (index != 0)
                result = entry - 16;
            else
                result = count * 16 + base - 16;
        }
        ++index;
    } while (result == 0);
    return static_cast<int>(result);
}

HOOK(0x4E0870, CircularList_PrevOf, "auto");
