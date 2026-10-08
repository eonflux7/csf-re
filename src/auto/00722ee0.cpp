#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860,
         0x00714860, (CGuiListBox* self, void* edx));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex,
         0x00714CA0, (CGuiListBox* self, void* edx, int selectionIndex));

CSF_GLOBAL(float, g_CellTextColorSentinelRed,   0x008A7DA0);
CSF_GLOBAL(float, g_CellTextColorSentinelGreen, 0x008A7DA4);
CSF_GLOBAL(float, g_CellTextColorSentinelBlue,  0x008A7DA8);
CSF_GLOBAL(float, g_CellTextColorSentinelAlpha, 0x008A7DAC);

static float* __fastcall CGuiListBox_GetCellTextColor(
    CGuiListBox* self, void* /*edx*/, int rowIndex, int columnIndex)
{
    const std::uint32_t selectionCount =
        static_cast<std::uint32_t>(
            CGuiListBox_GetItemCount_714860(self, nullptr));

    std::uint32_t selectionIndex = 0;
    while (selectionIndex < selectionCount)
    {
        if (GetStoredValueAtIndex(
                self, nullptr, static_cast<int>(selectionIndex)) == rowIndex)
        {
            break;
        }
        ++selectionIndex;
    }

    const std::uint32_t rowAddress =
        self->field_0220 + static_cast<std::uint32_t>(rowIndex) * 16u;

    // The original fetches the row's cell pointer even for selected rows.
    const std::uint32_t cellsAddress =
        CSF_AT(volatile std::uint32_t,
               reinterpret_cast<void*>(rowAddress), 0x4);

    const std::uint32_t cellAddress =
        cellsAddress + static_cast<std::uint32_t>(columnIndex) * 60u;
    void* const cell = reinterpret_cast<void*>(cellAddress);
    float* const cellColor = reinterpret_cast<float*>(cellAddress + 8u);

    if (selectionIndex < selectionCount && self->field_0140 != 0)
        return self->m_selTextColor;

    if (CSF_AT(float, cell, 0x8) != g_CellTextColorSentinelRed)
        return cellColor;
    if (CSF_AT(float, cell, 0xC) != g_CellTextColorSentinelGreen)
        return cellColor;
    if (CSF_AT(float, cell, 0x10) != g_CellTextColorSentinelBlue)
        return cellColor;
    if (CSF_AT(float, cell, 0x14) != g_CellTextColorSentinelAlpha)
        return cellColor;

    return reinterpret_cast<float*>(&self->field_01f8);
}

HOOK(0x00722EE0, CGuiListBox_GetCellTextColor, "auto");
