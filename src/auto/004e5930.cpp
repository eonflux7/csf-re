#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Binary search over the C_0083e9d0 record table (20-byte records, key at +12).
// The search key is the dword at +4 of the argument record (a2).
// Returns the lower-bound index: the first record whose key is >= the search key,
// or the count when every record is smaller.
static int __fastcall C_0083e9d0_LowerBoundByKey(C_0083e9d0* self, void* /*edx*/, int a2) {
    int searchKey = CSF_AT(int, reinterpret_cast<uint8_t*>(a2), 4);

    int lo = 0;
    int hi = static_cast<int>(self->field_0028) - 1;

    if (hi >= 0) {
        uint8_t* records = reinterpret_cast<uint8_t*>(self->field_0020);
        do {
            int mid = (hi + lo) / 2;
            int recordKey = CSF_AT(int, records, 20 * mid + 12);
            if (searchKey <= recordKey) {
                if (searchKey >= recordKey) {
                    return lo;
                }
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } while (lo <= hi);
    }
    return lo;
}

HOOK(0x004E5930, C_0083e9d0_LowerBoundByKey, "auto");
