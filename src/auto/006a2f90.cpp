#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __fastcall, GrowAllocationRecordArray, 0x006D9EB0, (void* self, void* edx, int a2));

CSF_GLOBAL(std::uint8_t*, g_AllocationRecords, 0x008A6BAC);
CSF_GLOBAL(std::uint32_t, g_AllocationRecordCapacity, 0x008A6BB0);
CSF_GLOBAL(std::uint32_t, g_AllocationRecordCount, 0x008A6BB4);
CSF_GLOBAL(int, g_AllocationRecordGrowth, 0x008A6BB8);

static int __cdecl CoreZlibLog_AllocateReusableBlock(
    int /*context*/, int elementCount, int elementSize)
{
    const std::uint32_t size =
        static_cast<std::uint32_t>(elementCount) *
        static_cast<std::uint32_t>(elementSize);

    const std::uint32_t count = g_AllocationRecordCount;
    if (count != 0)
    {
        std::uint8_t* record = g_AllocationRecords;
        for (std::uint32_t index = 0; index < count; ++index, record += 12)
        {
            if (CSF_AT(std::uint8_t, record, 0x08) != 0 &&
                CSF_AT(std::uint32_t, record, 0x04) == size)
            {
                record = g_AllocationRecords + 12 * index;
                const int allocation = CSF_AT(int, record, 0x00);
                CSF_AT(std::uint8_t, record, 0x08) = 0;
                if (allocation != 0)
                    return allocation;
                break;
            }
        }
    }

    SetAllocationMode(3);
    const int allocation = Mem_AllocDebug(size, 0, 0);
    SetAllocationMode(0);

    std::uint32_t index = g_AllocationRecordCount;
    if (index >= g_AllocationRecordCapacity)
    {
        GrowAllocationRecordArray(
            &g_AllocationRecords, nullptr, g_AllocationRecordGrowth);
        index = g_AllocationRecordCount;
    }

    std::uint8_t* record = g_AllocationRecords + 12 * index;
    g_AllocationRecordCount = index + 1;
    CSF_AT(int, record, 0x00) = allocation;
    CSF_AT(std::uint32_t, record, 0x04) = size;
    CSF_AT(std::uint32_t, record, 0x08) = 0;
    return allocation;
}

HOOK(0x006A2F90, CoreZlibLog_AllocateReusableBlock, "auto");
