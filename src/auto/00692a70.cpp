#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, S_0069b1c0_ReleaseOwnedBuffer, 0x0069B0C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_0069b1c0_ClearPositionBit, 0x0069B1C0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(S_0069b1c0**, g_ParserAllocationBlocks, 0x008A66B4);
CSF_GLOBAL(uint32_t, g_ParserAllocationBlockCount, 0x008A66BC);

// The packet also identifies the bit-clear entry address as a vtable reference.
[[maybe_unused]] static const auto g_PositionBitTable = CSF_VTBL(0x0069B1C0);

static void __cdecl CoreParser_ReleaseAllocation(S_0069b1c0* allocation)
{
    uint32_t index = g_ParserAllocationBlockCount;
    while (index != 0)
    {
        S_0069b1c0* candidate = g_ParserAllocationBlocks[index - 1];
        --index;
        if (!S_0069b1c0_ClearPositionBit(
                candidate, nullptr,
                static_cast<unsigned int>(reinterpret_cast<uintptr_t>(allocation))))
        {
            continue;
        }

        S_0069b1c0* block = g_ParserAllocationBlocks[index];
        if (block->field_001c != 0)
            return;

        if (block)
        {
            S_0069b1c0_ReleaseOwnedBuffer(block, nullptr);
            Mem_Free(block);
            g_ParserAllocationBlocks[index] = nullptr;
        }

        uint32_t remainingCount = g_ParserAllocationBlockCount - 1;
        g_ParserAllocationBlockCount = remainingCount;
        if (index != remainingCount)
        {
            S_0069b1c0** destination = g_ParserAllocationBlocks + index;
            _memcpy(destination, destination + 1, 4 * (remainingCount - index));
        }
        return;
    }

    if (allocation)
        Mem_Free(allocation);
}

HOOK(0x00692A70, CoreParser_ReleaseAllocation, "auto");
