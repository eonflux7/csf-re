#include <cstdint>
#include "hook.h"

// Walks backward from srcEnd/dstEnd, 8 bytes per step, until the source cursor
// reaches stopAddr. Each 8-byte slot holds a pointer to a 64-byte block; the
// block at the source pointer is copied into the block at the destination pointer.
// Returns the destination cursor.
static int __cdecl CopyPointedBlocksBackward(int stopAddr, int srcEnd, int dstEnd) {
    uint32_t srcCursor = static_cast<uint32_t>(srcEnd);
    uint32_t dstCursor = static_cast<uint32_t>(dstEnd);
    const uint32_t stop = static_cast<uint32_t>(stopAddr);

    while (srcCursor != stop) {
        const uint32_t srcBlockAddr = *reinterpret_cast<const uint32_t*>(static_cast<uintptr_t>(srcCursor - 8));
        const uint32_t dstBlockAddr = *reinterpret_cast<const uint32_t*>(static_cast<uintptr_t>(dstCursor - 8));
        srcCursor -= 8;
        dstCursor -= 8;

        const uint32_t* srcBlock = reinterpret_cast<const uint32_t*>(static_cast<uintptr_t>(srcBlockAddr));
        uint32_t* dstBlock = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(dstBlockAddr));
        for (int i = 0; i < 16; ++i) {
            dstBlock[i] = srcBlock[i];
        }
    }
    return static_cast<int>(dstCursor);
}

HOOK(0x006B3DC0, CopyPointedBlocksBackward, "auto");
