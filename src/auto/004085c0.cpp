#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall MatrixArray_GrowCapacity(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(28u * newCapacity + 4u));
    std::uint8_t* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = newCapacity;
        newEntries = allocation + 4;

        if (static_cast<std::int32_t>(newCapacity - 1u) >= 0)
        {
            auto* entry = newEntries;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(std::uint32_t*, entry, 0x8) = RwMatrixCreate();
                CSF_AT(std::uint8_t, entry, 0xC) = 1;
                CSF_AT(std::uint32_t, entry, 0x10) = 0;
                CSF_AT(std::uint32_t, entry, 0x14) = 0;
                CSF_AT(std::uint32_t, entry, 0x18) = 0;
                entry += 28;
            } while (--remaining);
        }
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount != 0)
    {
        std::memcpy(newEntries, CSF_AT(void*, self, 0x0),
                    28u * usedCount);
    }

    CSF_AT(std::uint32_t, self, 0x4) +=
        static_cast<std::uint32_t>(additionalCapacity);

    auto* oldEntries = CSF_AT(std::uint8_t*, self, 0x0);
    if (oldEntries)
    {
        auto* oldAllocation = oldEntries - 4;
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldAllocation, 0x0);

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            auto* entry = oldEntries + 28u * oldCapacity;
            std::uint32_t remaining = oldCapacity;
            do
            {
                entry -= 28;
                if (CSF_AT(std::uint8_t, entry, 0xC))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, entry, 0x8));
            } while (--remaining);
        }

        Mem_Free(oldAllocation);
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newEntries;
    return newEntries;
}

HOOK(0x004085C0, MatrixArray_GrowCapacity, "auto");
