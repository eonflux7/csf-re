#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns the int stored at +0x48 of the first entry (80-byte records in the
// array whose begin pointer lives at this+0x08 and end at this+0x0C) whose
// flag byte at +0x4C is nonzero. If none is flagged, the value of the entry at
// index == count (one past the last) is returned, as the original does.
static int __fastcall CProfileManager_GetFirstFlaggedEntryValue(uint32_t* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    uint32_t entryBegin = CSF_AT(uint32_t, base, 0x08);
    int32_t entryCount = 0;
    if (entryBegin != 0) {
        entryCount = static_cast<int32_t>(CSF_AT(uint32_t, base, 0x0C) - entryBegin) / 80;
    }

    uint32_t index = 0;
    if (entryCount != 0) {
        uint32_t count = static_cast<uint32_t>(entryCount);
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, base, 0x08))) + 76;
        do {
            if (*entry) {
                break;
            }
            ++index;
            entry += 80;
        } while (index < count);
    }

    uint32_t currentBegin = CSF_AT(uint32_t, base, 0x08);
    uint8_t* entries = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(currentBegin));
    return CSF_AT(int, entries, index * 80 + 72);
}

HOOK(0x515C20, CProfileManager_GetFirstFlaggedEntryValue, "auto");
