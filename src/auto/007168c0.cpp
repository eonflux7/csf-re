#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiListBoxBase_EnsureColumn, 0x00716490,
         (CGuiListBoxBase* self, void* edx, std::int32_t columnIndex));

static void __fastcall CGuiListBoxBase_SetColumnObject(
    CGuiListBoxBase* self, void* /*edx*/, std::int32_t columnIndex,
    void* replacement)
{
    if (columnIndex < 0)
        return;

    self->m_bLayoutDirty = 1;
    CGuiListBoxBase_EnsureColumn(self, nullptr, columnIndex);

    // Retained from the original despite the preceding negative-index guard.
    if (columnIndex == -1) {
        const std::uint32_t begin = self->m_columnsBegin;
        std::int32_t columnCount = 0;
        if (begin != 0) {
            columnCount =
                static_cast<std::int32_t>(self->m_columnsEnd - begin) / 12;
        }
        columnIndex = static_cast<std::int32_t>(
            static_cast<std::uint32_t>(columnCount) - 1u);
    }

    const std::uint32_t byteOffset =
        static_cast<std::uint32_t>(columnIndex) * 12u;
    const std::uint32_t columnAddress =
        self->m_columnsBegin + byteOffset;
    void* previous =
        CSF_AT(void*, reinterpret_cast<void*>(columnAddress), 0x04);

    if (previous != nullptr) {
        ((void(__fastcall*)(void*, void*, std::uint32_t))
             CSF_AT(void**, previous, 0x00)[0])(previous, nullptr, 1);

        const std::uint32_t refreshedAddress =
            self->m_columnsBegin + byteOffset;
        CSF_AT(void*, reinterpret_cast<void*>(refreshedAddress), 0x04) =
            nullptr;
    }

    const std::uint32_t destinationAddress =
        self->m_columnsBegin + byteOffset;
    CSF_AT(void*, reinterpret_cast<void*>(destinationAddress), 0x04) =
        replacement;
}

HOOK(0x007168C0, CGuiListBoxBase_SetColumnObject, "auto");
