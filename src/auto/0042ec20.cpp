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

// Linear search over a table of {key, value} dword pairs.
// table+0: pointer to the first entry, table+8: number of entries.
// Returns the value paired with the first entry whose key equals `key`, or 0 if none matches.
static int __stdcall HudTable_LookupValueByKey(int key, int table)
{
    const uint32_t entryCount = CSF_AT(uint32_t, table, 0x8);
    int result = 0;

    if (entryCount == 0)
        return result;

    const uint32_t* entries = reinterpret_cast<const uint32_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, table, 0x0)));

    for (uint32_t index = 0; index < entryCount; ++index)
    {
        const uint32_t* entry = entries + index * 2;
        if (static_cast<int>(entry[0]) == key)
        {
            result = static_cast<int>(entry[1]);
            return result;
        }
    }

    return result;
}

HOOK(0x42EC20, HudTable_LookupValueByKey, "auto");
