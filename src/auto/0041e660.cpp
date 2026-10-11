#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Binary search over the sorted uint32 array at field_0004 (count at field_000c).
// Returns the index of key if found; otherwise the insertion index (first element > key).
static int __fastcall C_0082e664_FindLowerBoundIndex(C_0082e664* self, void* /*edx*/, uint32_t key) {
    int low = 0;
    int high = (int)self->field_000c - 1;
    if (high < 0)
        return 0;

    uint32_t* keys = reinterpret_cast<uint32_t*>(self->field_0004);
    do {
        int mid = (high + low) / 2;
        uint32_t midValue = keys[mid];
        if (key <= midValue) {
            if (key >= midValue)
                return low;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    } while (low <= high);

    return low;
}

HOOK(0x41E660, C_0082e664_FindLowerBoundIndex, "auto");
