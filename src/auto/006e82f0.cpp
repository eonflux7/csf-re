#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Walks the per-node flag table of an HAnim hierarchy (node list at a1+0x10,
// 16-byte entries, flags at entry+0x18). Flag bit 2 pushes the current index
// onto a small stack; bit 1 pops it into the result, otherwise the result is
// the current index. Returns the last computed result (0 if a2 <= 1).
static int HAnim_ResolveStackOrder(int a1, int a2) {
    // Original stack buffer: 0x190 bytes of ints. Index 0 is the base slot
    // (never written before a read, so it is zero here, matching "pass 0").
    int stack[0x190 / 4] = {};

    int result = 0;
    int index = 1;

    if (a2 > 1) {
        void* obj = reinterpret_cast<void*>(a1);
        uint8_t* nodeTable = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, obj, 0x10));
        int* flagPtr = reinterpret_cast<int*>(nodeTable + 0x18);
        int top = 0;

        do {
            int flags = *flagPtr;

            if ((flags & 2) != 0) {
                ++top;
                stack[top] = index;
            }

            if ((flags & 1) != 0) {
                result = stack[top];
                --top;
            } else {
                result = index;
            }

            ++index;
            flagPtr += 4;
        } while (index < a2);
    }

    return result;
}

HOOK(0x006E82F0, HAnim_ResolveStackOrder, "auto");
