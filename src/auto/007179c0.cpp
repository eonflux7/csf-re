#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004a31e0_GetTwelveByteEntryCount, 0x004A31E0, (S_004a31e0* self, void*));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_UpdateSliderFromVisibleRowCount, 0x00714310, (CGuiListBoxBase* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AppendRows, 0x007178D0, (CGuiListBoxBase* self, void* , std::int32_t count));

static std::uint8_t __fastcall CGuiListBoxBase_PrepareVisibleRows(
    CGuiListBoxBase* self, void* /*edx*/)
{
    std::uint8_t result = self->m_bLayoutDirty;
    if (!result)
        return result;
    result = self->vIsSet();
    if (!result)
        return result;

    self->m_bLayoutDirty = 0;
    self->field_0174 = 0;
    self->field_0178 = 0;

    alignas(8) std::uint8_t desiredSizeBytes[0x8] = {};
    alignas(8) std::uint8_t adjustedSizeBytes[0x10] = {};
    auto& desiredWidth = CSF_AT(std::uint32_t, desiredSizeBytes, 0);
    auto& desiredHeight = CSF_AT(std::uint32_t, desiredSizeBytes, 4);

    self->m_rowHeight =
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x270 / 4])(self, nullptr);

    if (!self->m_bAutoSize)
    {
        std::uint32_t begin = self->m_columnsBegin;
        if (begin &&
            static_cast<std::int32_t>(self->m_columnsEnd - begin) / 12 != 0)
        {
            const std::int32_t availableWidth =
                CSF_AT(std::int32_t, self->vGetSize(), 0);

            // This address is also identified as a vtable reference in the packet.
            (void)CSF_VTBL(0x004A31E0);
            const int columnCount = S_004a31e0_GetTwelveByteEntryCount(
                reinterpret_cast<S_004a31e0*>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x198),
                nullptr);

            std::uint32_t fixedWidth = 0;
            int emptyColumns = 0;
            if (static_cast<std::uint32_t>(columnCount) > 0)
            {
                auto* column = reinterpret_cast<std::uint8_t*>(self->m_columnsBegin);
                std::uint32_t remaining = columnCount;
                do
                {
                    const std::uint32_t width = CSF_AT(std::uint32_t, column, 0);
                    if (width)
                        fixedWidth += width;
                    else
                        ++emptyColumns;
                    column += 12;
                } while (--remaining);
            }

            if (static_cast<std::int32_t>(fixedWidth) < availableWidth)
            {
                std::uint32_t distributedWidth = 0;
                if (emptyColumns > 0)
                {
                    const std::int32_t share =
                        static_cast<std::int32_t>(
                            static_cast<std::uint32_t>(availableWidth) - fixedWidth) /
                        emptyColumns;
                    std::uint32_t remaining = columnCount;
                    std::uint32_t offset = 0;
                    if (remaining)
                    {
                        do
                        {
                            auto* column = reinterpret_cast<std::uint8_t*>(
                                self->m_columnsBegin) + offset;
                            if (!CSF_AT(std::uint32_t, column, 0))
                            {
                                CSF_AT(std::uint32_t, column, 0) += share;
                                distributedWidth += share;
                            }
                            offset += 12;
                        } while (--remaining);
                    }
                }

                if (static_cast<std::int32_t>(fixedWidth + distributedWidth) <
                    availableWidth)
                {
                    std::uint32_t count = self->m_columnsBegin;
                    if (count)
                        count = static_cast<std::int32_t>(
                            self->m_columnsEnd - count) / 12;
                    auto* lastColumn = reinterpret_cast<void*>(
                        self->m_columnsBegin + 12 * count - 12);
                    CSF_AT(std::uint32_t, lastColumn, 0) +=
                        static_cast<std::uint32_t>(availableWidth) -
                        distributedWidth - fixedWidth;
                }
            }
        }
    }

    void* header = reinterpret_cast<void*>(self->m_pHeaderText);
    if (header)
    {
        void* metrics =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, header, 0)[0x28 / 4])(header, nullptr);
        const int height =
            ((int (__fastcall*)(void*, void*, int))
                CSF_AT(void**, metrics, 0)[0x18 / 4])(metrics, nullptr, 1);
        self->field_0174 = height + 2 * self->m_lineSpacing;
    }

    std::uint32_t columnCount = self->m_columnsBegin;
    if (columnCount)
        columnCount = static_cast<std::int32_t>(
            self->m_columnsEnd - columnCount) / 12;
    if (columnCount)
    {
        std::uint32_t offset = 0;
        do
        {
            void* label = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t,
                    reinterpret_cast<std::uint8_t*>(self->m_columnsBegin) + offset,
                    4));
            if (label)
            {
                void* metrics =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, label, 0)[0x28 / 4])(label, nullptr);
                const int height =
                    ((int (__fastcall*)(void*, void*, int))
                        CSF_AT(void**, metrics, 0)[0x18 / 4])(metrics, nullptr, 1);
                if (height > static_cast<std::int32_t>(self->field_0178))
                    self->field_0178 = height + 2 * self->m_lineSpacing;
            }
            desiredWidth += CSF_AT(std::uint32_t,
                reinterpret_cast<std::uint8_t*>(self->m_columnsBegin) + offset,
                0);
            offset += 12;
        } while (--columnCount);
    }

    desiredHeight = self->field_0178 + self->field_0174;

    if (self->m_bRowFill && static_cast<std::int32_t>(self->m_rowHeight) > 0)
    {
        void* size = self->vGetSize();
        const std::int32_t availableHeight = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, size, 4) -
            self->field_0174 - self->field_0178);
        const std::int32_t visibleRows = availableHeight /
            static_cast<std::int32_t>(
                self->m_rowHeight + 2 * self->m_lineSpacing);

        bool appended = false;
        if (visibleRows == -1)
        {
            CGuiListBoxBase_AppendRows(self, nullptr, 1);
            appended = true;
        }
        else
        {
            const int rows =
                ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
            if (visibleRows >= rows)
            {
                const int currentRows =
                    ((int (__fastcall*)(void*, void*))
                        CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
                CGuiListBoxBase_AppendRows(
                    self, nullptr, visibleRows - currentRows + 1);
                appended = true;
            }
        }
        if (appended)
        {
            self->m_bLayoutDirty = 1;
            CGuiListBoxBase_UpdateSliderFromVisibleRowCount(self, nullptr);
        }

        const std::int32_t remainder = static_cast<std::int32_t>(
            static_cast<std::uint32_t>(availableHeight) -
            static_cast<std::uint32_t>(visibleRows) *
                (self->m_rowHeight + 2 * self->m_lineSpacing));
        if (remainder > 0)
        {
            const std::uint32_t adjustedHeight =
                CSF_AT(std::uint32_t, self->vGetSize(), 4) - remainder;
            void* currentSize = self->vGetSize();
            CSF_AT(std::uint32_t, adjustedSizeBytes, 0) =
                CSF_AT(std::uint32_t, currentSize, 0);
            CSF_AT(std::uint32_t, adjustedSizeBytes, 4) = adjustedHeight;
            self->vSetSize(reinterpret_cast<int*>(adjustedSizeBytes));
        }
    }

    result = self->m_bAutoSize;
    if (result)
    {
        if (self->vIsSet_9a())
        {
            const int rowHeight =
                ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x270 / 4])(self, nullptr);
            std::uint32_t rows =
                ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
            if (rows)
            {
                std::uint32_t height = desiredHeight;
                do
                {
                    height += rowHeight;
                } while (--rows);
                desiredHeight = height;
            }
        }
        return static_cast<std::uint8_t>(
            self->vSetSize(reinterpret_cast<int*>(desiredSizeBytes)));
    }
    return result;
}

HOOK(0x007179C0, CGuiListBoxBase_PrepareVisibleRows, "auto");
