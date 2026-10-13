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

// Binary search over a sorted table of 16-byte entries. Each entry starts with
// a (major, minor) pair of dwords; its sort key is major * 1000 + minor.
// keyPair[0] = major, keyPair[1] = minor. Returns the entry index, or -1.
static int __fastcall S_004dc8f0_FindKeyIndex(S_004dc8f0* self, void* /*edx*/, int32_t* keyPair) {
    if (keyPair[0] == -1)
        return -1;
    int32_t minor = keyPair[1];
    if (minor == -1)
        return -1;

    uint32_t searchKey = static_cast<uint32_t>(minor) + 1000u * static_cast<uint32_t>(keyPair[0]);

    uint32_t tableBegin = self->field_0068;
    int32_t entryCount = 0;
    if (tableBegin != 0)
        entryCount = static_cast<int32_t>(self->field_006c - tableBegin) >> 4;

    int32_t high = entryCount - 1;
    int32_t low = 0;
    uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(tableBegin));

    while (low <= high) {
        int32_t mid = (low + high) / 2;
        uint32_t entryKey = CSF_AT(uint32_t, table, mid * 16) * 1000u
                          + CSF_AT(uint32_t, table, mid * 16 + 4);
        if (searchKey <= entryKey) {
            if (searchKey >= entryKey)
                return mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return -1;
}

HOOK(0x4DC8F0, S_004dc8f0_FindKeyIndex, "auto");
