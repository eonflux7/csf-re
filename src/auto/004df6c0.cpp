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

// Min-heap insertion: places (payload, key) at holeIndex and sifts it up toward
// topIndex, moving parents down while their key is greater than `key`.
// Each heap slot is 8 bytes: int payload at +0, float key at +4.
static int __cdecl MinHeapInsertSiftUp(int heapBase, int holeIndex, int topIndex,
                                       int payload, float key, int /*unused*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(heapBase);
    int hole = holeIndex;
    int parent = (hole - 1) / 2;

    if (topIndex < holeIndex) {
        do {
            float parentKey = CSF_AT(float, base, parent * 8 + 4);
            if (parentKey <= key)
                break;

            // Move parent entry down into the hole.
            CSF_AT(int, base, hole * 8) = CSF_AT(int, base, parent * 8);
            CSF_AT(int, base, hole * 8 + 4) = CSF_AT(int, base, parent * 8 + 4);

            hole = parent;
            bool keepGoing = topIndex < parent;
            parent = (parent - 1) / 2;
            if (!keepGoing)
                break;
        } while (true);
    }

    CSF_AT(int, base, hole * 8) = payload;
    CSF_AT(float, base, hole * 8 + 4) = key;
    return payload;
}

HOOK(0x4DF6C0, MinHeapInsertSiftUp, "auto");
