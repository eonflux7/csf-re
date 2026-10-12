#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns true when the entry with the given index is "not blocking" for the
// pending-index lookup: flag bit 2 always blocks, flag bit 4 blocks only while
// the position (+0x10) is below the threshold (+0x18). Not found -> true.
static bool __fastcall DynamicObj_IsPendingIndexMatch(void* self, void* /*edx*/, int targetIndex)
{
    uint8_t* matchedEntry = nullptr;

    // Count is read once before the loop (the original keeps it in ebx).
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x14);

    for (uint32_t i = 0; i < entryCount; ++i)
    {
        // The entry array is re-read on each iteration, as the original does.
        uint32_t* entrySlots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x0C));
        uint8_t* entry = reinterpret_cast<uint8_t*>(entrySlots[i]);

        if (CSF_AT(int, entry, 0x08) == targetIndex)
            matchedEntry = entry;

        if (matchedEntry)
        {
            const uint32_t flags = CSF_AT(uint32_t, matchedEntry, 0x0C);
            if ((flags & 2) != 0)
                return true;
            if ((flags & 4) != 0 &&
                CSF_AT(float, matchedEntry, 0x10) >= CSF_AT(float, matchedEntry, 0x18))
                return true;
            return false;
        }
    }

    return true;
}

HOOK(0x443230, DynamicObj_IsPendingIndexMatch, "auto");
