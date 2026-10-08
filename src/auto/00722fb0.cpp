#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ExtendToRowIndex, 0x00717970,
         (CGuiListBox* self, void* edx, int row));
CSF_FUNC(void, __fastcall, EnsureColumn, 0x00716490,
         (CGuiListBox* self, void* edx, int column));
CSF_FUNC(int, __fastcall, GetColumnCount, 0x00714830,
         (CGuiListBox* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AssignWideString, 0x00696F90,
         (void* self, void* edx, int16_t* text));
CSF_FUNC(void, __fastcall, SetCellValue, 0x00715570,
         (CGuiListBox* self, void* edx, int row, int column, int value));

static void __fastcall CGuiListBox_SetCellTextAndValue(
    CGuiListBox* self, void* /*edx*/,
    int row, int column, int16_t* text, int value)
{
    ExtendToRowIndex(self, nullptr, row);
    EnsureColumn(self, nullptr, column);

    if (row == -1) {
        const int rowCount =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
        row = static_cast<int>(static_cast<uint32_t>(rowCount) - 1u);
    }

    if (column == -1) {
        column = static_cast<int>(
            static_cast<uint32_t>(GetColumnCount(self, nullptr)) - 1u);
    }

    auto* rowRecord = reinterpret_cast<uint8_t*>(self->field_0220)
                    + static_cast<uint32_t>(row) * 16u;
    auto* cell = reinterpret_cast<uint8_t*>(
                     CSF_AT(uint32_t, rowRecord, 0x04))
               + static_cast<uint32_t>(column) * 60u;

    AssignWideString(cell + 0x18, nullptr, text);
    SetCellValue(self, nullptr, row, column, value);
}

HOOK(0x00722FB0, CGuiListBox_SetCellTextAndValue, "auto");
