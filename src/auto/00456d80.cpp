#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Scans the entry array for the first entry whose key (+0x04) equals a2.
// If found, increments that entry's use counter (+0x30) and returns the new value.
// If not found, returns the entry count.
static unsigned int __fastcall C_008312e0_BumpEntryUseCount(C_008312e0* self, void* /*edx*/, int a2) {
    const uint32_t entryCount = self->field_0028;
    uint8_t* matchedEntry = nullptr;
    uint32_t index = 0;

    while (index < entryCount) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(
            reinterpret_cast<uint32_t*>(self->field_0020)[index]);
        if (static_cast<uint32_t>(a2) == CSF_AT(uint32_t, entry, 0x04)) {
            matchedEntry = entry;
        }
        ++index;
        if (matchedEntry) {
            break;
        }
    }

    if (matchedEntry == nullptr) {
        return index;
    }

    const uint32_t newUseCount = CSF_AT(uint32_t, matchedEntry, 0x30) + 1;
    CSF_AT(uint32_t, matchedEntry, 0x30) = newUseCount;
    return newUseCount;
}

HOOK(0x00456D80, C_008312e0_BumpEntryUseCount, "auto");
