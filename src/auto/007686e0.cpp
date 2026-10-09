#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Entries are 24-byte records; the first dword of each record is the key.
// Returns the index of the first entry whose key equals `key`, or -1.
static int __fastcall InputBindings_FindEntryIndexByKey(void* self, void* /*edx*/, int key) {
    const uint32_t entriesBegin = CSF_AT(uint32_t, self, 0x08);
    uint32_t entryCount = 0;
    if (entriesBegin != 0) {
        const int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) - entriesBegin);
        entryCount = static_cast<uint32_t>(byteSpan / 24);
    }

    uint32_t index = 0;
    if (entryCount == 0) {
        return -1;
    }

    uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    while (key != CSF_AT(int, entry, 0)) {
        ++index;
        if (index >= entryCount) {
            return -1;
        }
        entry += 24;
    }
    return static_cast<int>(index);
}

HOOK(0x7686E0, InputBindings_FindEntryIndexByKey, "auto");
