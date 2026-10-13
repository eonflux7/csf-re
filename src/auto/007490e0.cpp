#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Unlinks a node from a circular doubly-linked list (sentinel at self+4, count at self+8),
// releases the node's side buffer (at +0x14) when it is in the large-buffer state (+0x28 >= 8),
// resets the node, frees it, and returns the next node through outNext.
static uint32_t* __fastcall ListContainer_RemoveNode(uint8_t* self, void* /*edx*/, uint32_t* outNext, uint8_t* node)
{
    uint32_t nextNode = CSF_AT(uint32_t, node, 0);

    if (reinterpret_cast<uint32_t>(node) != CSF_AT(uint32_t, self, 4))
    {
        uint32_t prevNode = CSF_AT(uint32_t, node, 4);

        // Splice the node out of the list.
        CSF_AT(uint32_t, prevNode, 0) = nextNode;
        CSF_AT(uint32_t, nextNode, 4) = prevNode;

        // Free the side buffer if the node is in the large-buffer state.
        uint32_t buffer = CSF_AT(uint32_t, node, 0x14);
        if (CSF_AT(uint32_t, node, 0x28) >= 8u && buffer != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(buffer)));
        }

        CSF_AT(uint32_t, node, 0x28) = 7;
        CSF_AT(uint32_t, node, 0x24) = 0;
        CSF_AT(uint16_t, node, 0x14) = 0;

        Mem_FreeDebug(node);
        --CSF_AT(uint32_t, self, 8);
    }

    *outNext = nextNode;
    return outNext;
}

HOOK(0x007490E0, ListContainer_RemoveNode, "auto");
