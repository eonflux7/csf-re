#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: 0x004DF6C0 (__cdecl, 6 args, ret 0). Restores heap order from a hole position.
CSF_FUNC(int, __cdecl, MaxHeap_RestoreOrder, 0x004DF6C0,
         (int a1, int a2, int a3, int a4, float a5, int a6));

// Heap of 8-byte entries: int at +0, float key at +4.
// Moves the hole at a2 down the tree choosing the larger child, until it reaches
// the bottom (index >= a3). If the hole lands exactly on the last index, the last
// entry is moved into the hole. Then the hole index is passed to MaxHeap_RestoreOrder.
static int __cdecl MaxHeap_PercolateHoleDown(int a1, int a2, int a3, int a4, float a5, int a6)
{
    uint8_t* entries = reinterpret_cast<uint8_t*>(a1);
    int holeIndex = a2;
    int childIndex = 2 * a2 + 2;
    bool hitLastIndex = (childIndex == a3);

    while (childIndex < a3)
    {
        // Pick the right child (2i+2) unless the left child (2i+1) has the larger key.
        if (CSF_AT(float, entries, 8 * childIndex + 4) > CSF_AT(float, entries, 8 * childIndex - 4))
            --childIndex;

        CSF_AT(int, entries, 8 * holeIndex) = CSF_AT(int, entries, 8 * childIndex);
        CSF_AT(int, entries, 8 * holeIndex + 4) = CSF_AT(int, entries, 8 * childIndex + 4);

        holeIndex = childIndex;
        childIndex = 2 * childIndex + 2;
        hitLastIndex = (childIndex == a3);
    }

    if (hitLastIndex)
    {
        CSF_AT(int, entries, 8 * holeIndex) = CSF_AT(int, entries, 8 * a3 - 8);
        CSF_AT(int, entries, 8 * holeIndex + 4) = CSF_AT(int, entries, 8 * a3 - 4);
        holeIndex = a3 - 1;
    }

    return MaxHeap_RestoreOrder(a1, holeIndex, a2, a4, a5, a6);
}

HOOK(0x004DF740, MaxHeap_PercolateHoleDown, "auto");
