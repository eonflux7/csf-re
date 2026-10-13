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

// Binary search over a sorted array of uint32_t keys stored in the object.
// Returns the index of `key` in the array, or -1 if it is absent or the array is empty.
static int __fastcall C_008311f0_FindIndexInSortedArray(C_008311f0* self, void* /*edx*/, uint32_t key)
{
    const uint32_t count = self->field_000c;
    if (count == 0)
        return -1;

    int low = 0;
    int high = static_cast<int>(count) - 1;
    if (high < 0)
        return -1;

    const uint32_t* sortedKeys = reinterpret_cast<const uint32_t*>(self->field_0004);

    do {
        int mid = (high + low) / 2;
        uint32_t midKey = sortedKeys[mid];

        if (key <= midKey) {
            if (key >= midKey)
                return mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    } while (low <= high);

    return -1;
}

HOOK(0x0044D9B0, C_008311f0_FindIndexInSortedArray, "auto");
