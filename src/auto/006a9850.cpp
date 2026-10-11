#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Walks the circular list rooted at the sentinel stored at +0x54 (count at +0x58).
// Node layout: [0] next, [4] prev, [8] payload.
// index == 0 returns the first node's payload; index > 0 returns the payload of the
// index-th node after the sentinel, or 0 if the walk reaches the sentinel first.
static int __fastcall NetGatheringDdl_GetListEntryData(void* self, void* /*edx*/, int index) {
    int result = 0;

    if (CSF_AT(uint32_t, self, 0x58) != 0) {
        uint8_t* sentinel = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x54));
        uint8_t* node = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, sentinel, 0));

        if (index == 0) {
            return CSF_AT(int, node, 8);
        }

        int position = 0;
        while (true) {
            node = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, node, 0));
            if (node == sentinel) {
                break;
            }
            if (++position == index) {
                return CSF_AT(int, node, 8);
            }
        }
    }

    return result;
}

HOOK(0x6A9850, NetGatheringDdl_GetListEntryData, "auto");
