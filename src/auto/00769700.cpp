#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Searches the table of input-binding entries for the first entry whose leading
// key dword equals `actionKey`. Each entry is 68 bytes (17 dwords). The table
// spans [begin, end) stored at self+0x8 and self+0xC. Returns the entry index,
// or -1 if the table is empty or the key is absent.
static int __fastcall InputBindings_FindActionIndex(void* self, void* /*edx*/, int actionKey)
{
    const uint32_t tableBegin = CSF_AT(uint32_t, self, 0x8);
    const uint32_t tableEnd = CSF_AT(uint32_t, self, 0xC);

    uint32_t entryCount = 0;
    if (tableBegin != 0) {
        // Signed element count: (end - begin) / sizeof(entry) with sizeof(entry) == 68.
        const int byteSpan = static_cast<int>(tableEnd - tableBegin);
        entryCount = static_cast<uint32_t>(byteSpan / 68);
    }

    int index = 0;
    if (entryCount == 0) {
        return -1;
    }

    const int* entry = reinterpret_cast<const int*>(tableBegin);
    while (*entry != actionKey) {
        ++index;
        if (static_cast<uint32_t>(index) >= entryCount) {
            return -1;
        }
        entry += 17;
    }
    return index;
}

HOOK(0x00769700, InputBindings_FindActionIndex, "auto");
