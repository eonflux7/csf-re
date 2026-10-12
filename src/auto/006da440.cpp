#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, GrowRecordArray, 0x006D9EB0, (void* self, void* edx, int a2));

static int __fastcall RecordList_InsertAfter(
    void* self, void* /*edx*/, const std::uint32_t* value)
{
    void* array = CSF_AT(void*, self, 0x04);
    const std::uint32_t newIndex = CSF_AT(std::uint32_t, array, 0x08);

    if (newIndex >= CSF_AT(std::uint32_t, array, 0x04))
        GrowRecordArray(array, nullptr, CSF_AT(int, array, 0x0C));

    const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x08);
    auto* record = CSF_AT(std::uint8_t*, array, 0x00) + 12 * count;
    CSF_AT(std::uint32_t, array, 0x08) = count + 1;
    CSF_AT(std::uint32_t, record, 0x00) = 0;
    CSF_AT(std::uint32_t, record, 0x04) = 0;
    CSF_AT(std::uint32_t, record, 0x08) = 0;

    auto* records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
    const std::uint32_t payload = *value;
    const std::uint32_t newOffset = 12 * newIndex;
    CSF_AT(std::uint32_t, records + newOffset, 0x00) = payload;

    if (newIndex == 0)
    {
        CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x04), 0x10) = 0;
        CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x04), 0x14) = 0;

        records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
        CSF_AT(std::int32_t, records + newOffset, 0x04) = -1;
        records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
        CSF_AT(std::int32_t, records + newOffset, 0x08) = -1;
        return static_cast<int>(newOffset);
    }

    const std::int32_t currentIndex = CSF_AT(std::int32_t, self, 0x00);
    if (currentIndex == -1)
    {
        records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
        CSF_AT(std::int32_t, records + newOffset, 0x04) = -1;

        array = CSF_AT(void*, self, 0x04);
        records = CSF_AT(std::uint8_t*, array, 0x00);
        const std::uint32_t head = CSF_AT(std::uint32_t, array, 0x14);
        CSF_AT(std::uint32_t, records + newOffset, 0x08) = head;

        array = CSF_AT(void*, self, 0x04);
        const std::uint32_t oldHead = CSF_AT(std::uint32_t, array, 0x14);
        records = CSF_AT(std::uint8_t*, array, 0x00);
        CSF_AT(std::uint32_t, records + 12 * oldHead, 0x04) = newIndex;
        CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x04), 0x14) = newIndex;
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(records));
    }

    records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
    CSF_AT(std::int32_t, records + newOffset, 0x04) = currentIndex;

    records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
    const std::uint32_t insertionIndex = CSF_AT(std::uint32_t, self, 0x00);
    const std::uint32_t successor =
        CSF_AT(std::uint32_t, records + 12 * insertionIndex, 0x08);
    CSF_AT(std::uint32_t, records + newOffset, 0x08) = successor;

    const std::uint32_t currentOffset = 12 * CSF_AT(std::uint32_t, self, 0x00);
    records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
    if (CSF_AT(std::int32_t, records + currentOffset, 0x08) == -1)
    {
        CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x04), 0x10) = newIndex;
    }
    else
    {
        records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
        const std::uint32_t nextIndex =
            CSF_AT(std::uint32_t, records + currentOffset, 0x08);
        CSF_AT(std::uint32_t, records + 12 * nextIndex, 0x04) = newIndex;
    }

    const std::uint32_t finalIndex = CSF_AT(std::uint32_t, self, 0x00);
    records = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x04), 0x00);
    CSF_AT(std::uint32_t, records + 12 * finalIndex, 0x08) = newIndex;
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(records));
}

HOOK(0x006DA440, RecordList_InsertAfter, "auto");
