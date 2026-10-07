#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall GetIndexedTableValue(
    int* self, void* /*edx*/, int rowIndex, int columnIndex)
{
    if (rowIndex < 0)
        return -1;

    const int rowCount =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

    if (rowIndex >= rowCount)
        return -1;

    if (columnIndex < 0)
        return -1;

    const std::uint32_t columnsBegin = CSF_AT(std::uint32_t, self, 0x19C);
    int columnCount = 0;
    if (columnsBegin != 0)
    {
        const std::uint32_t byteDifference =
            CSF_AT(std::uint32_t, self, 0x1A0) - columnsBegin;
        columnCount = static_cast<std::int32_t>(byteDifference) / 12;
    }

    if (columnIndex >= columnCount)
        return -1;

    const std::uint32_t rowAddress =
        CSF_AT(std::uint32_t, self, 0x18C) +
        (static_cast<std::uint32_t>(rowIndex) << 4);
    const std::uint32_t valuesAddress =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(rowAddress), 4);

    return CSF_AT(
        int, reinterpret_cast<void*>(valuesAddress),
        static_cast<std::uint32_t>(columnIndex) * 4u);
}

HOOK(0x00714C20, GetIndexedTableValue, "auto");
