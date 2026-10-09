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

typedef unsigned char (__cdecl *SortPredicateFn)(int, int);

CSF_FUNC(int, __cdecl, SortMedian3, 0x769010, (int* a, int* b, int* c, SortPredicateFn pred));

// Median-of-N pivot selection used by the sort: for large ranges takes the
// median of three medians-of-three, otherwise a single median of three.
static int __cdecl InputBindings_SortMedian(int* first, int* mid, int* last, SortPredicateFn pred)
{
    int count = (int)(last - first);
    if (count <= 40) {
        return SortMedian3(first, mid, last, pred);
    }

    int step = (count + 1) / 8;
    int* firstStep = first + step;

    SortMedian3(first, firstStep, first + 2 * step, pred);
    SortMedian3(mid - step, mid, mid + step, pred);
    int* lastStep = last - step;
    SortMedian3(last - 2 * step, lastStep, last, pred);
    return SortMedian3(firstStep, mid, lastStep, pred);
}

HOOK(0x007691D0, InputBindings_SortMedian, "auto");
