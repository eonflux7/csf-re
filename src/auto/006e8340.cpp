#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Engine instance; its vtable slot 66 (offset 0x108) is the RW allocator.
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x8B68C0);

// __cdecl allocator slot in the RW engine vtable: (byteCount, zero).
using RwEngineAllocFn = void* (__cdecl*)(int byteCount, int zero);

// Builds, for every node of an HAnim hierarchy, the index of its parent.
// Each node carries a 16-byte record whose flags dword (+8) says:
//   bit 1 (0x2): this node opens a level, so the current parent is pushed;
//   bit 0 (0x1): this node closes a level, so the parent is popped;
//   neither:     this node becomes the current parent for the following nodes.
static int* __cdecl HAnim_BuildParentIndexTable(void* hierarchy) {
    RwEngineAllocFn allocate = reinterpret_cast<RwEngineAllocFn>(
        CSF_AT(void**, g_pRwEngineInstance, 0)[66]);
    int* outIndices = static_cast<int*>(
        allocate(4 * CSF_AT(int, hierarchy, 4), 0));

    if (CSF_AT(uint32_t, hierarchy, 4) == 0) {
        return outIndices;
    }

    // Parent stack; the original frame reserves 0x190 bytes = 100 ints.
    int stackValues[100];
    int* stackTop = stackValues;

    uint32_t currentParent = 0;
    uint32_t nodeIndex = 0;
    do {
        outIndices[nodeIndex] = static_cast<int>(currentParent);

        uint8_t* records = CSF_AT(uint8_t*, hierarchy, 0x10);
        uint32_t flags = *reinterpret_cast<uint32_t*>(
            records + static_cast<size_t>(nodeIndex) * 16 + 8);

        if ((flags & 2) != 0) {
            *++stackTop = static_cast<int>(currentParent);
        }
        if ((flags & 1) != 0) {
            currentParent = static_cast<uint32_t>(*stackTop--);
        } else {
            currentParent = nodeIndex;
        }

        ++nodeIndex;
    } while (nodeIndex < CSF_AT(uint32_t, hierarchy, 4));

    return outIndices;
}

HOOK(0x6E8340, HAnim_BuildParentIndexTable, "auto");
