#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
static std::uint8_t __stdcall BddWaypointGraph_DestroyNodeTree(void** block);  // self-recursion through the same convention

// __stdcall, ret 4: the single node pointer is popped by the callee.
// Walks the sibling chain at +0x00 until the end flag at +0x31 is set,
// recursing into the child subtree at +0x08 and freeing each node after the one before it.
static std::uint8_t __stdcall BddWaypointGraph_DestroyNodeTree(void** block)
{
    void** node = block;
    void** nodeToFree = block;
    std::uint8_t endFlag = CSF_AT(std::uint8_t, node, 0x31);

    while (endFlag == 0)
    {
        BddWaypointGraph_DestroyNodeTree(CSF_AT(void**, node, 0x08));
        node = CSF_AT(void**, node, 0x00);

        if (nodeToFree != nullptr)
            Mem_FreeDebug(nodeToFree);

        endFlag = CSF_AT(std::uint8_t, node, 0x31);
        nodeToFree = node;
    }

    return endFlag;
}

HOOK(0x0075EDC0, BddWaypointGraph_DestroyNodeTree, "auto");
