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

// Comparator passed through the heap helpers: returns nonzero when the first element should sit below the second.
using HeapElementCompareFn = std::uint8_t(__cdecl*)(int, int);

CSF_FUNC(int, __cdecl, Heap_SiftUpToPosition, 0x769070,
         (int a1, int a2, int a3, int a4, HeapElementCompareFn a5));

static int __cdecl Heap_SiftDownWithComparator(int a1, int a2, int a3, int a4, HeapElementCompareFn a5)
{
    int* heap = reinterpret_cast<int*>(static_cast<std::intptr_t>(a1));
    int hole = a2;
    int child = 2 * a2 + 2;

    if (child < a3)
    {
        do
        {
            if (a5(heap[child], heap[child - 1]))
                --child;

            heap[hole] = heap[child];
            hole = child;
            child = 2 * child + 2;
        } while (child < a3);
    }

    if (child == a3)
    {
        heap[hole] = heap[a3 - 1];
        hole = a3 - 1;
    }

    return Heap_SiftUpToPosition(a1, hole, a2, a4, a5);
}

HOOK(0x769280, Heap_SiftDownWithComparator, "auto");
