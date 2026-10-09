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

// Comparator passed through to the heap helper (cdecl, two args: element value, int).
using InputBindingsComparator = uint8_t (__cdecl*)(uint32_t, int);

// Sift-down/adjust helper of the original (std::__adjust_heap equivalent). Caller cleans the stack (cdecl, 5 args).
CSF_FUNC(int, __cdecl, InputBindings_AdjustHeap, 0x769280,
         (uint32_t* first, int hole, int len, uint32_t value, InputBindingsComparator comp));

// Heap-sort finishing loop: repeatedly moves the current maximum to the end and re-adjusts the heap
// over the shrinking prefix. `last` is the one-past-end pointer (as an int), `comp` is the ordering predicate.
static int __cdecl InputBindings_SortHeap(uint32_t* first, int last, InputBindingsComparator comp)
{
    int byteSpan = last - (int)first;
    int result = byteSpan >> 2;
    if (result > 1)
    {
        do
        {
            uint32_t lastValue = *(uint32_t*)((char*)first + byteSpan - 4);
            *(uint32_t*)((char*)first + byteSpan - 4) = *first;
            InputBindings_AdjustHeap(first, 0, (byteSpan - 4) >> 2, lastValue, comp);
            byteSpan -= 4;
            result = byteSpan >> 2;
        }
        while ((byteSpan >> 2) > 1);
    }
    return result;
}

HOOK(0x769C20, InputBindings_SortHeap, "auto");
