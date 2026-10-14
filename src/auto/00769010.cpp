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

// Comparator supplied by the caller: returns nonzero when the first argument
// should be ordered after the second (swap wanted).
using SortComparatorFn = uint8_t(__cdecl*)(int, int);

// Sorts three int slots in place using the caller's comparator.
// Returns the comparator's result from the final check, or, when that check
// triggered a swap, the value that was in *second before the swap.
static int __cdecl SortThreeByComparator(int* firstSlot, int* secondSlot, int* thirdSlot, SortComparatorFn compare)
{
    // Order first and second.
    if (compare(*secondSlot, *firstSlot)) {
        int tmp = *secondSlot;
        *secondSlot = *firstSlot;
        *firstSlot = tmp;
    }

    // Order second and third.
    if (compare(*thirdSlot, *secondSlot)) {
        int tmp = *thirdSlot;
        *thirdSlot = *secondSlot;
        *secondSlot = tmp;
    }

    // Final check on first and second.
    int result = compare(*secondSlot, *firstSlot);
    if ((uint8_t)result) {
        result = *secondSlot;
        *secondSlot = *firstSlot;
        *firstSlot = result;
    }
    return result;
}

HOOK(0x769010, SortThreeByComparator, "auto");
