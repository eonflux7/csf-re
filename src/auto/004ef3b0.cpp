#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the 32-byte entry whose dwords [0], [1], [5] and [7] match the four
// arguments, shifting the following entries down by one slot.
static void __fastcall TextDatabase_RemoveMatchingEntry(uint32_t* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x24);
    uint32_t matchIndex = 0;

    if (entryCount != 0)
    {
        uint8_t* entryBase = CSF_AT(uint8_t*, self, 0x1C);
        uint32_t* entry = reinterpret_cast<uint32_t*>(entryBase);

        for (; entry[0] != static_cast<uint32_t>(a2)
            || entry[1] != static_cast<uint32_t>(a3)
            || entry[5] != static_cast<uint32_t>(a4)
            || entry[7] != static_cast<uint32_t>(a5);
            entry += 8)
        {
            if (++matchIndex >= entryCount)
                return;
        }

        const uint32_t remainingCount = entryCount - 1;
        CSF_AT(uint32_t, self, 0x24) = remainingCount;

        if (matchIndex != remainingCount)
        {
            uint8_t* matchedEntry = CSF_AT(uint8_t*, self, 0x1C) + 32 * matchIndex;
            _memcpy(matchedEntry, matchedEntry + 32, static_cast<std::size_t>(32 * (remainingCount - matchIndex)));
        }
    }
}

HOOK(0x004EF3B0, TextDatabase_RemoveMatchingEntry, "auto");
