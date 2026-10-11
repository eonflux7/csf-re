#include <cstdint>

#include "hook.h"
#include "core/memory.h"

// Walks the 31-entry cheat table (stride 0x3C, starting at +0x0C) and, for each of the
// three slots per entry, matches a (key1, key2) pair. On a match it sets that slot's
// flag to 1 and clears the counter that follows it. Returns a pointer past the table.
static int32_t* __fastcall Cheats_MarkMatchingEntries(void* self, void* /*edx*/, int32_t a2, int32_t a3)
{
    int32_t* result = reinterpret_cast<int32_t*>(self) + 3;
    int remainingEntries = 31;

    do
    {
        if (result[-2] == a2 && result[-1] == a3)
        {
            result[0] = 1;
            result[2] = 0;
        }
        if (result[3] == a2 && result[4] == a3)
        {
            result[5] = 1;
            result[7] = 0;
        }
        if (result[8] == a2 && result[9] == a3)
        {
            result[10] = 1;
            result[12] = 0;
        }
        result += 15;
        --remainingEntries;
    } while (remainingEntries != 0);

    return result;
}

HOOK(0x428680, Cheats_MarkMatchingEntries, "auto");
