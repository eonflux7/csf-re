#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee bound as given in the packet; processes the matched entry (ecx = entry, no stack args).
CSF_FUNC(int, __fastcall, ProcessMatchedEntry, 0x0040B770, (void* self, void* edx));

static uint32_t __fastcall FindEntryByKeyAndProcess(void* self, void* /*edx*/, int key) {
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x24);
    uint8_t* matchedEntry = nullptr;
    uint32_t index = 0;

    for (; index < entryCount; ++index) {
        // The entry array pointer is re-read on every iteration, as the original does.
        const uint32_t entryArrayBase = CSF_AT(uint32_t, self, 0x1C);
        uint8_t* entry = reinterpret_cast<uint8_t*>(reinterpret_cast<uint32_t*>(entryArrayBase)[index]);

        if (CSF_AT(uint32_t, entry, 0x8) == static_cast<uint32_t>(key)) {
            matchedEntry = entry;
        }
        if (matchedEntry) {
            return static_cast<uint32_t>(ProcessMatchedEntry(matchedEntry, nullptr));
        }
    }

    if (matchedEntry) {
        return static_cast<uint32_t>(ProcessMatchedEntry(matchedEntry, nullptr));
    }
    return index;
}

HOOK(0x408540, FindEntryByKeyAndProcess, "auto");
