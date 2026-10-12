#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(char, __fastcall, SpatialGrid_GetObjectCellBounds, 0x004DE230, (void* self, void* edx, int a2, uint32_t* a3, uint32_t* a4, uint32_t* a5, uint32_t* a6));
CSF_FUNC(char, __stdcall, SpatialGrid_CellIntersectsObject, 0x004DE2B0, (void* a1, void* a2, void* a3));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_SpatialGridCellSize, 0x0083E108); // 512.0f

static uint32_t __fastcall S_004ddf20_InsertObjectIntoCells(
    S_004ddf20* self, void* /*edx*/, C_0086709c* object)
{
    // Native unsigned-to-float conversion replaces the original bias adjustment.
    (void)g_UnsignedIntegerBias;
    // The packet also lists the growth function's address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    alignas(8) uint8_t maximumBytes[0x8];
    alignas(8) uint8_t minimumBytes[0x10];
    uint32_t firstColumn = 0;
    uint32_t currentRow = 0;
    uint32_t lastColumn = 0;
    uint32_t lastRow = 0;

    SpatialGrid_GetObjectCellBounds(
        self, nullptr, reinterpret_cast<int>(object),
        &firstColumn, &currentRow, &lastColumn, &lastRow);

    uint32_t result = currentRow;
    uint32_t row = currentRow;
    while (row <= lastRow)
    {
        uint32_t column = firstColumn;
        if (column <= lastColumn)
        {
            CSF_AT(float, minimumBytes, 0xC) =
                static_cast<float>(row) * g_SpatialGridCellSize;

            do
            {
                CSF_AT(uint32_t, minimumBytes, 0x8) = column;
                CSF_AT(float, minimumBytes, 0x0) =
                    static_cast<float>(column) * g_SpatialGridCellSize +
                    self->field_000c;
                CSF_AT(float, minimumBytes, 0x4) =
                    CSF_AT(float, minimumBytes, 0xC) + self->field_0014;
                CSF_AT(float, maximumBytes, 0x0) =
                    CSF_AT(float, minimumBytes, 0x0) + g_SpatialGridCellSize;
                CSF_AT(float, maximumBytes, 0x4) =
                    CSF_AT(float, minimumBytes, 0x4) + g_SpatialGridCellSize;

                if (SpatialGrid_CellIntersectsObject(
                        minimumBytes, maximumBytes, object))
                {
                    const uint32_t cellAddress =
                        self->field_001c +
                        16 * (column + currentRow * self->field_0004);
                    auto* cell = reinterpret_cast<S_0041e5f0*>(cellAddress);
                    if (cell)
                    {
                        if (cell->field_0008 >= cell->field_0004)
                        {
                            S_0041e5f0_GrowArrayCapacity(
                                cell, nullptr,
                                static_cast<int>(cell->field_000c));
                        }

                        const uint32_t count = cell->field_0008;
                        auto** destination = reinterpret_cast<C_0086709c**>(
                            cell->field_0000 + 4 * count);
                        cell->field_0008 = count + 1;
                        *destination = object;
                    }
                }

                ++column;
            } while (column <= lastColumn);

            row = currentRow;
        }

        result = lastRow;
        ++row;
        currentRow = row;
    }

    return result;
}

HOOK(0x004DE430, S_004ddf20_InsertObjectIntoCells, "auto");
