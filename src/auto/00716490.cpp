#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiListBoxBase_AppendColumns, 0x007163F0,
         (CGuiListBoxBase* self, void* edx, std::int32_t count));

static void __fastcall CGuiListBoxBase_EnsureColumn(
    CGuiListBoxBase* self, void* /*edx*/, std::int32_t columnIndex)
{
    self->m_bLayoutDirty = 1;

    if (columnIndex == -1) {
        CGuiListBoxBase_AppendColumns(self, nullptr, 1);
        return;
    }

    std::uint32_t begin = self->m_columnsBegin;
    std::int32_t columnCount = 0;
    if (begin != 0) {
        columnCount =
            static_cast<std::int32_t>(self->m_columnsEnd - begin) / 12;
    }

    if (columnIndex < columnCount)
        return;

    begin = self->m_columnsBegin;
    columnCount = 0;
    if (begin != 0) {
        columnCount =
            static_cast<std::int32_t>(self->m_columnsEnd - begin) / 12;
    }

    const std::int32_t additionalCount = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(columnIndex) -
        static_cast<std::uint32_t>(columnCount) + 1u);
    CGuiListBoxBase_AppendColumns(self, nullptr, additionalCount);
}

HOOK(0x00716490, CGuiListBoxBase_EnsureColumn, "auto");
