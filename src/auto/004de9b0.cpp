#include <cstddef>
#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Owner layout (no class declared, so accessed by offset):
//   +0x00 void* records    (buffer of 12-byte records)
//   +0x04 int   capacity   (record count; grows by extraRecords)
//   +0x08 int   usedCount  (records copied from the old buffer)

static const void* __fastcall Vec_GrowTwelveByteRecordArray(void* self, void* /*edx*/, int extraRecords)
{
    // New capacity = current capacity + extra, computed before allocating.
    const std::int32_t newCapacity = CSF_AT(std::int32_t, self, 0x04) + extraRecords;

    void* newRecords = nullptr;
    void* allocated = Mem_Alloc(static_cast<std::size_t>(newCapacity) * 12u);
    if (allocated != nullptr)
    {
        newRecords = allocated;
        if (newCapacity > 0)
        {
            // Zero all three dwords of every 12-byte record.
            auto* bytes = static_cast<std::uint8_t*>(newRecords);
            for (std::int32_t i = 0; i < newCapacity; ++i)
            {
                auto* record = reinterpret_cast<std::uint32_t*>(bytes + static_cast<std::size_t>(i) * 12u);
                record[0] = 0;
                record[1] = 0;
                record[2] = 0;
            }
        }
    }

    // Copy the used records from the old buffer into the new one.
    const std::int32_t usedCount = CSF_AT(std::int32_t, self, 0x08);
    if (usedCount != 0)
    {
        const void* oldSource = CSF_AT(const void*, self, 0x00);
        std::memcpy(newRecords, oldSource, static_cast<std::size_t>(usedCount) * 12u);
    }

    void* oldRecords = CSF_AT(void*, self, 0x00);
    const void* result = oldRecords;
    const bool hadOldRecords = (oldRecords != nullptr);

    CSF_AT(std::int32_t, self, 0x04) += extraRecords;

    if (hadOldRecords)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::intptr_t>(Mem_Free(oldRecords)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newRecords;
    return result;
}

HOOK(0x004DE9B0, Vec_GrowTwelveByteRecordArray, "auto");
