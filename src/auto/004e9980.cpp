#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Empties a circular intrusive list whose sentinel node pointer is stored at self+0x04.
// Node layout: [0] = next, [4] = prev. Every node after the sentinel is freed with
// Mem_FreeDebug; the sentinel itself is not freed. self+0x08 (element count) is zeroed.
// Returns the sentinel pointer.
static uint32_t* __fastcall ClearListAndFreeNodes(uint32_t* self, void* /*edx*/)
{
    uint32_t* listHead = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04));
    uint32_t* currentNode = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, listHead, 0x00));

    // Make the sentinel an empty ring again.
    CSF_AT(uint32_t, listHead, 0x00) = reinterpret_cast<uint32_t>(listHead);
    CSF_AT(uint32_t, reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04)), 0x04) =
        CSF_AT(uint32_t, self, 0x04);
    CSF_AT(uint32_t, self, 0x08) = 0;

    if (currentNode != reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04))) {
        do {
            uint32_t* nextNode = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, currentNode, 0x00));
            Mem_FreeDebug(currentNode);
            currentNode = nextNode;
        } while (currentNode != reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04)));
    }

    return currentNode;
}

HOOK(0x4E9980, ClearListAndFreeNodes, "auto");
