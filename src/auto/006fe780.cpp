#include <cstddef>
#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall GfxClump_GrowTwentyFourByteRecordArray(
    void* self, void* /*edx*/, int extraRecords)
{
    const std::int32_t newCapacity =
        CSF_AT(std::int32_t, self, 0x04) + extraRecords;

    void* newRecords = Mem_Alloc(static_cast<std::size_t>(newCapacity) * 24u);
    if (newRecords != nullptr && newCapacity > 0)
    {
        // The cursor points to the last dword of each record.
        auto* cursor = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uint8_t*>(newRecords) + 0x14);
        std::int32_t remaining = newCapacity;
        do
        {
            cursor[-2] = 0;
            cursor[-1] = 0;
            cursor[0] = 0;
            cursor += 6;
            --remaining;
        } while (remaining != 0);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount != 0)
    {
        const void* oldSource = CSF_AT(const void*, self, 0x00);
        std::memcpy(newRecords, oldSource,
                    static_cast<std::size_t>(usedCount) * 24u);
    }

    void* oldRecords = CSF_AT(void*, self, 0x00);
    const void* result = oldRecords;

    CSF_AT(std::int32_t, self, 0x04) += extraRecords;

    if (oldRecords != nullptr)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::intptr_t>(Mem_Free(oldRecords)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newRecords;
    return result;
}

HOOK(0x006FE780, GfxClump_GrowTwentyFourByteRecordArray, "auto");
