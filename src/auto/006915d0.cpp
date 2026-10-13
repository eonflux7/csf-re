#include <cstdint>
#include <cstddef>
#include "hook.h"

// Lower-bound search over a sorted range of uint32 values [first, last).
// Returns a pointer to the first element >= value. Like the original, when
// the search ends at `last` the final dereference reads *lo unconditionally.
static unsigned int* __stdcall CoreMemory_LowerBoundU32(unsigned int* first,
                                                        unsigned int* last,
                                                        unsigned int value) {
    unsigned int* lo = first;
    unsigned int* hi = last;

    while (lo != hi) {
        std::ptrdiff_t count = hi - lo;
        unsigned int* mid = lo + count / 2;
        if (*mid >= value) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    unsigned int* after = lo + 1;
    if (*lo >= value) {
        return lo;
    }
    return after;
}

HOOK(0x6915D0, CoreMemory_LowerBoundU32, "auto");
