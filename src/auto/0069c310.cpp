// AppMain_ParseLanguageValue: looks up a language key in the parser table and
// returns the associated value, or 0 if no entry matches.
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Table of 12-byte entries: [0] = uint32 value, [4] = const char* key, [8] = unused.
// Begin and end addresses come from the original's data references.

static uint32_t __cdecl AppMain_ParseLanguageValue(const char* key)
{
    uint32_t result = 0;
    const uintptr_t tableBegin = reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008A66C8));
    const uintptr_t tableEnd   = reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008A677C));

    uintptr_t cursor = tableBegin;
    do
    {
        // Signed comparison, as in the original (jge).
        if (static_cast<int32_t>(cursor) >= static_cast<int32_t>(tableEnd))
            break;

        const char* entryKey = *reinterpret_cast<const char* const*>(cursor + 4);

        // Inline string equality (same as strcmp == 0).
        const unsigned char* a = reinterpret_cast<const unsigned char*>(key);
        const unsigned char* b = reinterpret_cast<const unsigned char*>(entryKey);
        bool equal = true;
        for (;;)
        {
            if (*a != *b) { equal = false; break; }
            if (*a == 0) break;
            ++a;
            ++b;
        }

        if (equal)
            result = *reinterpret_cast<const uint32_t*>(cursor);

        cursor += 12;
    } while (result == 0);

    return result;
}

HOOK(0x0069C310, AppMain_ParseLanguageValue, "auto");
