#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
static std::uint8_t __stdcall BddLoader_DestroyBlockTree(void** block);  // self-recursion through the same convention

// __stdcall, ret 4: the single block pointer is popped by the callee.
// Walks the sibling chain at +0x00 until the sentinel flag at +0x11 is set,
// recursing into the child at +0x08 and freeing each block after the one before it.
static std::uint8_t __stdcall BddLoader_DestroyBlockTree(void** block)
{
    void** node = block;
    void** blockToFree = block;
    std::uint8_t sentinelFlag = CSF_AT(std::uint8_t, node, 0x11);

    while (sentinelFlag == 0)
    {
        BddLoader_DestroyBlockTree(CSF_AT(void**, node, 0x08));
        node = CSF_AT(void**, node, 0x00);

        if (blockToFree != nullptr)
            Mem_FreeDebug(blockToFree);

        sentinelFlag = CSF_AT(std::uint8_t, node, 0x11);
        blockToFree = node;
    }

    return sentinelFlag;
}

HOOK(0x0050EA80, BddLoader_DestroyBlockTree, "auto");
