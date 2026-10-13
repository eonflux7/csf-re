#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "hook.h"

// Bound by address: the original calls _memcpy (cdecl, 3 args, caller cleans up).
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the 32-byte entry whose fields [0], [1] and [3] match (a2, a3, a5) from the
// entry array at this+0x1c. Count lives at this+0x24. Entries are 32 bytes each.
static void __fastcall TextDatabase_ProcessAltCategoryBlock(void* self, void* /*edx*/, int a2, int a3, int a4, int a5, int a6)
{
    (void)a4;
    (void)a6;

    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x24);
    uint32_t index = 0;
    if (entryCount == 0)
        return;

    uint8_t* entryBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x1c));
    for (;;)
    {
        uint8_t* entry = entryBase + 32 * index;
        if (CSF_AT(int, entry, 0) == a2 &&
            CSF_AT(int, entry, 4) == a3 &&
            CSF_AT(int, entry, 0xC) == a5)
        {
            break;
        }
        ++index;
        if (index >= entryCount)
            return;
    }

    // Found the entry at 'index': shrink the count and shift the tail down by one entry.
    const uint32_t newCount = CSF_AT(uint32_t, self, 0x24) - 1;
    CSF_AT(uint32_t, self, 0x24) = newCount;
    if (index != newCount)
    {
        uint8_t* base = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x1c));
        uint8_t* dest = base + 32 * index;
        _memcpy(dest, dest + 32, 32 * (newCount - index));
    }
}

HOOK(0x004EF340, TextDatabase_ProcessAltCategoryBlock, "auto");
