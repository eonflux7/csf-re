// Module: core_win32_debug
// Looks up a language entry by its key (first dword) in the
// g_ParserAllocationBlocks table (12-byte entries) and returns the dword at
// entry+8, or 0 if the key is not found.

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

static int __cdecl AppConfig_GetLanguageName(int key)
{
    int result = 0;

    // Table cursor starts at g_ParserAllocationBlocks (0x8A66C8), stride 12 bytes.
    uint32_t cursor = CSF_ADDR_DATA(0x8A66C8);
    const uint32_t tableEnd = CSF_ADDR_DATA(0x8A677C);

    while (CSF_AT(uint32_t, cursor, 0) != static_cast<uint32_t>(key))
    {
        cursor += 12;
        if (static_cast<int32_t>(cursor) >= static_cast<int32_t>(tableEnd))
            return result;
    }

    // Found: cursor points at the matching entry; the name value is at entry+8.
    if (cursor != 0)
        result = CSF_AT(int, cursor, 8);

    return result;
}

HOOK(0x0069C3C0, AppConfig_GetLanguageName, "auto");
