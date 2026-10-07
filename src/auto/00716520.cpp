#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiListBoxBase_EnsureColumn, 0x00716490,
         (CGuiListBoxBase* self, void* edx, std::int32_t columnIndex));

static void __fastcall CGuiListBoxBase_SetColumnWidth(
    CGuiListBoxBase* self, void* /*edx*/, std::uint32_t columnIndex,
    std::int32_t width)
{
    if (width < 0)
        return;

    self->m_bLayoutDirty = 1;
    CGuiListBoxBase_EnsureColumn(
        self, nullptr, static_cast<std::int32_t>(columnIndex));

    const std::uint32_t begin = self->m_columnsBegin;
    if (begin == 0)
        return;

    const std::int32_t columnCount =
        static_cast<std::int32_t>(self->m_columnsEnd - begin) / 12;
    if (columnIndex >= static_cast<std::uint32_t>(columnCount))
        return;

    const std::uint32_t columnAddress =
        self->m_columnsBegin + columnIndex * 12u;
    CSF_AT(std::int32_t, reinterpret_cast<void*>(columnAddress), 0x00) =
        width;
}

HOOK(0x00716520, CGuiListBoxBase_SetColumnWidth, "auto");
