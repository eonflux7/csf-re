#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Writes a two-dword pair into a 20-byte table entry (5 dwords per entry).
// Entries at index >= 93 are ignored; the return value is the entry address
// (or the raw index cast to a pointer when out of range), matching the original.
static int32_t* __cdecl Cheats_SetEntry(int32_t tableBase, int32_t entryIndex, int32_t valueA, int32_t valueB)
{
    int32_t* result = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(entryIndex)));
    if (entryIndex < 93)
    {
        uint8_t* entryBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(tableBase)))
                            + static_cast<intptr_t>(entryIndex) * 20;
        result = reinterpret_cast<int32_t*>(entryBytes);
        result[0] = valueA;
        result[1] = valueB;
    }
    return result;
}

HOOK(0x4287D0, Cheats_SetEntry, "auto");
