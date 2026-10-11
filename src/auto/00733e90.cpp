#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall CopyRecordArray(void* self, void* /*edx*/, const void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);
        auto* newData = static_cast<std::uint8_t*>(Mem_Alloc(28 * capacity));

        if (newData && static_cast<std::int32_t>(capacity - 1) >= 0)
        {
            auto* record = newData;
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, record, 0x04) = 0;
                CSF_AT(std::uint32_t, record, 0x08) = 0;
                CSF_AT(std::uint32_t, record, 0x0C) = 0;
                CSF_AT(std::uint32_t, record, 0x10) = 0;
                CSF_AT(std::uint32_t, record, 0x14) = 0;
                CSF_AT(float, record, 0x18) = 1.0f;
                CSF_AT(std::uint32_t, record, 0x00) = 0;
                record += 28;
            } while (--remaining);
        }

        const std::uint32_t newCapacity = CSF_AT(std::uint32_t, source, 0x04);
        void* oldData = CSF_AT(void*, self, 0x00);
        CSF_AT(std::uint32_t, self, 0x04) = newCapacity;
        if (oldData)
        {
            Mem_Free(oldData);
            CSF_AT(void*, self, 0x00) = nullptr;
        }
        CSF_AT(void*, self, 0x00) = newData;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x08);
    if (count)
    {
        const void* sourceData = CSF_AT(const void*, source, 0x00);
        void* destinationData = CSF_AT(void*, self, 0x00);
        std::memcpy(destinationData, sourceData, 28 * count);
    }

    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) = CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x00733E90, CopyRecordArray, "auto");
