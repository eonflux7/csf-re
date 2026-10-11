#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Game heap free (debug variant); cdecl, one argument, ret 0.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Table of tracked blocks: 12-byte entries. Entry layout: [0] block pointer, [8] byte flag.
CSF_GLOBAL(uint8_t*, g_TrackedBlockTable, 0x008A6BAC);
CSF_GLOBAL(uint32_t, g_TrackedBlockCount, 0x008A6BB4);

static int __cdecl ZlibLog_MarkTrackedBlock(int /*a1*/, void* block) {
    uint32_t count = g_TrackedBlockCount;
    if (count != 0) {
        uint8_t* entry = g_TrackedBlockTable;
        uint32_t index = 0;
        while (*reinterpret_cast<void**>(entry) != block) {
            ++index;
            if (index >= count) {
                return Mem_FreeDebug(block);
            }
            entry += 12;
        }
        int result = 3 * static_cast<int>(index);
        *(g_TrackedBlockTable + 4 * result + 8) = 1;
        return result;
    }
    return Mem_FreeDebug(block);
}

HOOK(0x6A2EB0, ZlibLog_MarkTrackedBlock, "auto");
