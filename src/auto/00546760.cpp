#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_AssignStringRange, 0x00403B60, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static std::uint32_t CMenuExtras_WrapThumbnailIndex(
    CMenuExtras* self, std::uint32_t index)
{
    const std::uint32_t begin = self->field_0038;
    if (!begin ||
        index >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_003c - begin) / 68))
    {
        index %= 6;
    }
    return index;
}

static void CMenuExtras_LinkThumbnailNeighbors(
    CMenuExtras* self, std::uint32_t index,
    std::uint32_t up, std::uint32_t down,
    std::uint32_t left, std::uint32_t right)
{
    alignas(8) std::uint8_t firstStringBytes[0x24];
    alignas(8) std::uint8_t secondStringBytes[0x44];
    C_0082d410& firstString =
        *reinterpret_cast<C_0082d410*>(firstStringBytes);
    C_0082d410& secondString =
        *reinterpret_cast<C_0082d410*>(secondStringBytes);

    auto* entry = reinterpret_cast<std::uint8_t*>(
        self->field_0038 + 68 * index);

    CSF_AT(std::uint32_t, firstStringBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, firstStringBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&firstString.field_0004)[0] = 0;
    MenuExtras_AssignStringRange(
        &firstString, nullptr, entry + 4, 0,
        static_cast<std::uint32_t>(-1));

    void* widget = CSF_AT(void*, entry, 32);

    CSF_AT(std::uint32_t, secondStringBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, secondStringBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&secondString.field_0004)[0] = 0;
    MenuExtras_AssignStringRange(
        &secondString, nullptr, entry + 40, 0,
        static_cast<std::uint32_t>(-1));

    void* neighbor = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0038 + 68 * up), 32);
    void** widgetVtable = CSF_AT(void**, widget, 0);
    int neighborId =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, neighbor, 0)[116 / 4])(neighbor, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        widgetVtable[128 / 4])(widget, nullptr, neighborId);

    neighbor = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0038 + 68 * down), 32);
    widgetVtable = CSF_AT(void**, widget, 0);
    neighborId =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, neighbor, 0)[116 / 4])(neighbor, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        widgetVtable[132 / 4])(widget, nullptr, neighborId);

    neighbor = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0038 + 68 * left), 32);
    widgetVtable = CSF_AT(void**, widget, 0);
    neighborId =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, neighbor, 0)[116 / 4])(neighbor, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        widgetVtable[120 / 4])(widget, nullptr, neighborId);

    neighbor = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0038 + 68 * right), 32);
    widgetVtable = CSF_AT(void**, widget, 0);
    neighborId =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, neighbor, 0)[116 / 4])(neighbor, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        widgetVtable[124 / 4])(widget, nullptr, neighborId);

    if (CSF_AT(std::uint32_t, secondStringBytes, 0x18) >= 0x10)
        CoreMemory_Free(reinterpret_cast<void*>(secondString.field_0004));
    CSF_AT(std::uint32_t, secondStringBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, secondStringBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&secondString.field_0004)[0] = 0;

    if (CSF_AT(std::uint32_t, firstStringBytes, 0x18) >= 0x10)
        CoreMemory_Free(reinterpret_cast<void*>(firstString.field_0004));
    CSF_AT(std::uint32_t, firstStringBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, firstStringBytes, 0x14) = 0;
    reinterpret_cast<std::uint8_t*>(&firstString.field_0004)[0] = 0;
}

static int __fastcall CMenuExtras_ConfigureThumbnailNavigation(
    CMenuExtras* self, void* /*edx*/)
{
    int result = static_cast<std::int32_t>(self->field_0038);
    if (!result)
        return result;

    result = static_cast<std::int32_t>(
        self->field_003c - self->field_0038) / 68;
    if (!result)
        return result;

    std::uint32_t count = self->field_0038;
    if (count)
    {
        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_003c - count) / 68);
    }

    std::uint32_t rowCount = count / 6;
    std::uint32_t lastRowColumns = count % 6;
    if (lastRowColumns)
        ++rowCount;
    else
        lastRowColumns = 6;

    std::uint32_t row = 0;
    std::uint32_t column = 0;
    result = static_cast<std::int32_t>(rowCount - 1);

    if (rowCount != 1)
    {
        std::uint32_t rowStart = 0;
        std::uint32_t nextRow = 1;
        do
        {
            const std::uint32_t current =
                CMenuExtras_WrapThumbnailIndex(self, rowStart + column);
            const std::uint32_t up = CMenuExtras_WrapThumbnailIndex(
                self, column + 6 * ((rowCount + nextRow - 2) % rowCount));
            const std::uint32_t down = CMenuExtras_WrapThumbnailIndex(
                self, column + 6 * (nextRow % rowCount));
            const std::uint32_t left = CMenuExtras_WrapThumbnailIndex(
                self, rowStart + (column + 5) % 6);
            const std::uint32_t nextColumn = column + 1;
            const std::uint32_t right = CMenuExtras_WrapThumbnailIndex(
                self, rowStart + nextColumn % 6);

            CMenuExtras_LinkThumbnailNeighbors(
                self, current, up, down, left, right);

            column = nextColumn;
            if (column == 6)
            {
                column = 0;
                ++row;
                ++nextRow;
                rowStart += 6;
            }
            result = static_cast<std::int32_t>(rowCount - 1);
        }
        while (row < rowCount - 1);
    }

    if (column < lastRowColumns)
    {
        const std::uint32_t rowStart = 6 * row;
        std::uint32_t nextColumn = column + 1;
        std::uint32_t downIndex = column + 6 * ((row + 1) % rowCount);
        std::uint32_t upIndex =
            column + 6 * ((row + rowCount - 1) % rowCount);
        std::uint32_t currentIndex = rowStart + column;
        int remaining = static_cast<std::int32_t>(lastRowColumns - column);

        for (;;)
        {
            const std::uint32_t current =
                CMenuExtras_WrapThumbnailIndex(self, currentIndex);
            const std::uint32_t up =
                CMenuExtras_WrapThumbnailIndex(self, upIndex);
            const std::uint32_t down =
                CMenuExtras_WrapThumbnailIndex(self, downIndex);
            const std::uint32_t left = CMenuExtras_WrapThumbnailIndex(
                self, rowStart +
                    (lastRowColumns + nextColumn - 2) % lastRowColumns);
            const std::uint32_t right = CMenuExtras_WrapThumbnailIndex(
                self, rowStart + nextColumn % lastRowColumns);

            CMenuExtras_LinkThumbnailNeighbors(
                self, current, up, down, left, right);

            ++currentIndex;
            ++upIndex;
            ++downIndex;
            ++nextColumn;
            result = remaining - 1;
            --remaining;
            if (!remaining)
                return result;
        }
    }

    return result;
}

HOOK(0x00546760, CMenuExtras_ConfigureThumbnailNavigation, "auto");
