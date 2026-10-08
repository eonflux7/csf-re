#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(std::uint32_t, __fastcall, InsertRepeatedDwords, 0x00555B70,
         (void* self, void* edx, std::uint32_t* position,
          std::uint32_t count, std::uint32_t* value));
CSF_FUNC(int, __fastcall, InsertColumnRecords, 0x00715970,
         (void* self, void* edx, char* position,
          std::uint32_t count, std::uint32_t* value));

static void __fastcall CGuiListBoxBase_AppendColumns(
    CGuiListBoxBase* self, void* /*edx*/, std::int32_t count)
{
    if (count < 0)
        return;

    void** vtable = CSF_AT(void**, self, 0);
    self->m_bLayoutDirty = 1;

    const std::int32_t rowCount =
        reinterpret_cast<std::int32_t (__fastcall*)(void*, void*)>(
            vtable[0x274 / 4])(self, nullptr);

    if (rowCount > 0) {
        std::uint32_t defaultValue = 0xFFFFFFFFu;
        std::uint32_t rowOffset = 0;

        for (std::int32_t row = 0; row < rowCount; ++row) {
            reinterpret_cast<void (__fastcall*)(
                void*, void*, std::int32_t, std::int32_t)>(
                    CSF_AT(void**, self, 0)[0x284 / 4])(
                        self, nullptr, row, count);

            auto* rowStorage =
                reinterpret_cast<std::uint8_t*>(self->field_018c) + rowOffset;
            auto* insertionPosition = reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, rowStorage, 0x8));

            InsertRepeatedDwords(
                rowStorage, nullptr, insertionPosition,
                static_cast<std::uint32_t>(count), &defaultValue);

            rowOffset += 0x10;
        }
    }

    char* insertionPosition = reinterpret_cast<char*>(self->m_columnsEnd);
    std::uint32_t emptyColumn[3] = {0, 0, 0};

    InsertColumnRecords(
        reinterpret_cast<std::uint8_t*>(self) + 0x198,
        nullptr, insertionPosition,
        static_cast<std::uint32_t>(count), emptyColumn);
}

HOOK(0x007163F0, CGuiListBoxBase_AppendColumns, "auto");
