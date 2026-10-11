#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Searches the entry table held in this->field_0008 for an entry whose first
// dword equals `key`. Entries are 12 bytes; returns a pointer to the entry's
// payload (entry + 4), or 0 if the table is missing, empty, or has no match.
static int __fastcall C_0085e7b8_FindEntryPayloadByKey(C_0085e7b8* self, void* /*edx*/, int key) {
    uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0008);
    if (!table) {
        return 0;
    }

    uint32_t entryCount = CSF_AT(uint32_t, table, 0x8);
    if (entryCount == 0) {
        return 0;
    }

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, table, 0x0));
    for (uint32_t index = 0; index < entryCount; ++index) {
        uint8_t* entry = entryBase + index * 12;
        if (CSF_AT(int, entry, 0) == key) {
            return static_cast<int>(reinterpret_cast<uintptr_t>(entry) + 4);
        }
    }
    return 0;
}

HOOK(0x006C4870, C_0085e7b8_FindEntryPayloadByKey, "auto");
