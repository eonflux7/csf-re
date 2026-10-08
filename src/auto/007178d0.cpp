#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(std::uint32_t, __fastcall, ResizeDwordVector, 0x00715FD0,
         (void* self, void* edx, std::uint32_t count, int value));
CSF_FUNC(void*, __fastcall, InsertRowRecords, 0x00717440,
         (void* self, void* edx, std::uint32_t position,
          std::uint32_t count, void* value));
CSF_FUNC(int, __cdecl, FreeDebugMemory, 0x00690E60,
         (void* block));

static void __fastcall CGuiListBoxBase_AppendRows(
    CGuiListBoxBase* self, void* /*edx*/, std::int32_t count)
{
    if (count < 0)
        return;

    self->m_bLayoutDirty = 1;

    std::uint32_t columnCount = self->m_columnsBegin;
    if (columnCount != 0) {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            self->m_columnsEnd - columnCount);
        columnCount = static_cast<std::uint32_t>(byteCount / 12);
    }

    std::uint32_t temporaryRow[4];
    temporaryRow[1] = 0;
    temporaryRow[2] = 0;
    temporaryRow[3] = 0;

    ResizeDwordVector(temporaryRow, nullptr, columnCount, 0);

    InsertRowRecords(
        reinterpret_cast<std::uint8_t*>(self) + 0x188,
        nullptr, self->field_0190,
        static_cast<std::uint32_t>(count), temporaryRow);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::int32_t)>(
        CSF_AT(void**, self, 0)[0x288 / 4])(self, nullptr, count);

    if (temporaryRow[1] != 0)
        FreeDebugMemory(reinterpret_cast<void*>(temporaryRow[1]));
}

HOOK(0x007178D0, CGuiListBoxBase_AppendRows, "auto");
