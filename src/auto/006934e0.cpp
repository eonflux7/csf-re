#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __fastcall, InitObjectState, 0x0069AFD0, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, S_0069afe0_AllocateSlot, 0x0069AFE0, (S_0069afe0* self, void*));
CSF_FUNC(unsigned int, __fastcall, S_00471340_InitializePool, 0x0069B100, (S_00471340* self, void* , int elementCount, unsigned int requestedBits, char useCrtHeap));

CSF_GLOBAL(void**, g_ParserAllocationBlocks, 0x008A66B4);
CSF_GLOBAL(uint32_t, g_ParserAllocationBlockCapacity, 0x008A66B8);
CSF_GLOBAL(uint32_t, g_ParserAllocationBlockCount, 0x008A66BC);
CSF_GLOBAL(int, g_ParserAllocationBlockGrowth, 0x008A66C0);

static int __cdecl CorePaths_AllocateNodeData(std::size_t size)
{
    // These packet-listed vtable references actually identify direct callees.
    // Retain their bindings without reading or installing a vtable.
    (void)CSF_VTBL(0x0069AFE0);
    (void)CSF_VTBL(0x0069AFD0);
    (void)CSF_VTBL(0x0041E5F0);

    uint32_t remainingBlocks = g_ParserAllocationBlockCount;
    while (remainingBlocks != 0)
    {
        auto* block = reinterpret_cast<S_0069afe0*>(
            g_ParserAllocationBlocks[remainingBlocks - 1]);
        --remainingBlocks;

        int slot = S_0069afe0_AllocateSlot(block, nullptr);
        if (slot != 0)
            return slot;
    }

    int allocation = CoreMemory_AllocUntagged(44);
    S_00471340* newBlock = nullptr;
    if (allocation != 0)
    {
        newBlock = reinterpret_cast<S_00471340*>(
            InitObjectState(reinterpret_cast<uint8_t*>(allocation), nullptr));
    }

    S_00471340_InitializePool(newBlock, nullptr, 76, 0x64u, 0);
    int slot = S_0069afe0_AllocateSlot(
        reinterpret_cast<S_0069afe0*>(newBlock), nullptr);

    if (g_ParserAllocationBlockCount >= g_ParserAllocationBlockCapacity)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&g_ParserAllocationBlocks),
            nullptr, g_ParserAllocationBlockGrowth);
    }

    void** destination =
        g_ParserAllocationBlocks + g_ParserAllocationBlockCount;
    ++g_ParserAllocationBlockCount;
    *destination = newBlock;

    if (slot != 0)
        return slot;

    return Mem_AllocDebug(size, 0, 0);
}

HOOK(0x006934E0, CorePaths_AllocateNodeData, "auto");
