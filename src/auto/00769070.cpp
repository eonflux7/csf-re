#include <cstdint>
#include "hook.h"

// Sift-up step of a binary min-heap stored in an int array.
// Moves the hole at holeIndex toward the root while the parent compares "greater"
// (per the comparator), stopping at topIndex. Places value into the final hole.
static int __cdecl InputBindings_HeapSiftUp(int heapArray, int holeIndex, int topIndex,
                                            int value, bool (__cdecl* compare)(int, int)) {
    int* heap = reinterpret_cast<int*>(heapArray);
    int hole = holeIndex;
    int parent = (holeIndex - 1) / 2;

    while (topIndex < hole) {
        if (!compare(heap[parent], value)) {
            break;
        }
        heap[hole] = heap[parent];
        hole = parent;
        parent = (parent - 1) / 2;
    }

    heap[hole] = value;
    return value;
}

HOOK(0x769070, InputBindings_HeapSiftUp, "auto");
