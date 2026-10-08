#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiTextureListBox_ExtendToRowIndex, 0x00717970,
         (CGuiTextureListBox* self, void* edx, int row));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_EnsureColumn, 0x00716490,
         (CGuiListBoxBase* self, void* edx, int column));
CSF_FUNC(int, __fastcall, CGuiTextureListBox_GetColumnCount, 0x00714830,
         (CGuiTextureListBox* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiTextureListBox_SetCellValue, 0x00715570,
         (CGuiTextureListBox* self, void* edx, int row, int column, int value));

static void __fastcall CGuiTextureListBox_SetCellTextureAndValue(
    CGuiTextureListBox* self, void* /*edx*/,
    int row, int column, int texture, int value)
{
    CGuiTextureListBox_ExtendToRowIndex(self, nullptr, row);
    CGuiListBoxBase_EnsureColumn(self, nullptr, column);

    if (row == -1) {
        const int count =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
        row = static_cast<int>(static_cast<std::uint32_t>(count) - 1u);
    }

    if (column == -1) {
        const int count = CGuiTextureListBox_GetColumnCount(self, nullptr);
        column = static_cast<int>(static_cast<std::uint32_t>(count) - 1u);
    }

    const std::uint32_t rowAddress =
        self->field_01fc + (static_cast<std::uint32_t>(row) << 4);
    const std::uint32_t cells =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(rowAddress), 4);
    const std::uint32_t cellAddress =
        cells + static_cast<std::uint32_t>(column) * 4u;
    CSF_AT(int, reinterpret_cast<void*>(cellAddress), 0) = texture;

    CGuiTextureListBox_SetCellValue(self, nullptr, row, column, value);
}

HOOK(0x007309D0, CGuiTextureListBox_SetCellTextureAndValue, "auto");
