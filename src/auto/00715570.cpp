#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiTextureListBox_SetCellValue(
    CGuiTextureListBox* self, void* /*edx*/,
    int rowIndex, int columnIndex, std::uint32_t value)
{
    if (rowIndex < 0)
        return;

    const int rowCount =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

    if (rowIndex >= rowCount || columnIndex < 0)
        return;

    const std::uint32_t columnsBegin = self->m_columnsBegin;
    const int columnCount = columnsBegin
        ? static_cast<std::int32_t>(self->m_columnsEnd - columnsBegin) / 12
        : 0;

    if (columnIndex >= columnCount)
        return;

    const std::uint32_t rowAddress =
        self->field_018c + (static_cast<std::uint32_t>(rowIndex) << 4);
    const std::uint32_t cellsAddress = CSF_AT(
        std::uint32_t, reinterpret_cast<void*>(rowAddress), 0x4);

    const std::uint32_t cellAddress =
        cellsAddress + static_cast<std::uint32_t>(columnIndex) * 4u;
    *reinterpret_cast<std::uint32_t*>(cellAddress) = value;
}

HOOK(0x00715570, CGuiTextureListBox_SetCellValue, "auto");
