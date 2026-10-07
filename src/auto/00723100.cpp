#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall CGuiListBox_SetCellData(
    CGuiListBox* self, void* /*edx*/,
    int rowIndex, int columnIndex, std::uint32_t* source)
{
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

    const std::uint32_t rowAddress =
        self->field_0220 + (static_cast<std::uint32_t>(rowIndex) << 4);
    const std::uint32_t cellsAddress =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(rowAddress), 0x4);
    auto* destination = reinterpret_cast<std::uint32_t*>(
        cellsAddress + static_cast<std::uint32_t>(columnIndex) * 0x3Cu + 8u);

    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    destination[3] = source[3];
    return destination;
}

HOOK(0x00723100, CGuiListBox_SetCellData, "auto");
