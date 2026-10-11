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

// Scans a HAnim table for the first 16-byte entry whose leading dword equals `key`.
// Returns the entry index, or -1 if the table is empty or the key is absent.
static int HAnim_FindEntryIndexByKey(int table, int key) {
    void* tablePtr = reinterpret_cast<void*>(table);
    int entryCount = CSF_AT(int, tablePtr, 0x04);
    uint8_t* entry = CSF_AT(uint8_t*, tablePtr, 0x10);
    int index = 0;

    if (entryCount <= 0) {
        return -1;
    }

    while (*reinterpret_cast<int*>(entry) != key) {
        entry += 0x10;
        if (++index >= entryCount) {
            return -1;
        }
    }
    return index;
}

HOOK(0x6E8120, HAnim_FindEntryIndexByKey, "auto");
