#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CGuiListBoxBase_EnsureColumn, 0x00716490, (CGuiListBoxBase* self, void* , std::int32_t columnIndex));
CSF_FUNC(char, __fastcall, CGuiListBoxBase_PrepareVisibleRows, 0x007179C0, (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_SetPosition_7198A0, 0x007198A0, (CGuiControl* self, void* edx, float x, float y));

static std::uint32_t __fastcall CGuiListBoxBase_ScaleLayout(
    CGuiListBoxBase* self, void* /*edx*/, float scaleX, float scaleY)
{
    const auto horizontal = static_cast<std::int64_t>(
        static_cast<float>(static_cast<std::int32_t>(self->field_01d4)) * scaleX);
    const auto vertical = static_cast<std::int64_t>(
        static_cast<float>(static_cast<std::int32_t>(self->field_01d8)) * scaleY);
    const auto spacing = static_cast<std::int64_t>(
        static_cast<float>(static_cast<std::int32_t>(self->m_lineSpacing)) * scaleY);

    self->field_01d4 = static_cast<std::uint32_t>(horizontal);
    self->field_01d8 = static_cast<std::uint32_t>(vertical);
    self->m_lineSpacing = static_cast<std::uint32_t>(spacing);

    reinterpret_cast<void(__fastcall*)(void*, void*, float, float)>(
        self->vtbl[0x26C / 4])(self, nullptr, scaleX, scaleY);
    CGuiControl_SetPosition_7198A0(self, nullptr, scaleX, scaleY);

    std::uint32_t columnCount = 0;
    if (self->m_columnsBegin)
    {
        columnCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->m_columnsEnd - self->m_columnsBegin) / 12);
    }

    std::uint32_t columnIndex = 0;
    std::uint32_t columnOffset = 0;
    while (columnIndex < columnCount)
    {
        auto* column = reinterpret_cast<std::uint8_t*>(self->m_columnsBegin) + columnOffset;
        const auto scaledWidth = static_cast<std::int32_t>(
            static_cast<std::int64_t>(
                static_cast<float>(CSF_AT(std::int32_t, column, 0)) * scaleX));

        if (scaledWidth >= 0)
        {
            self->m_bLayoutDirty = 1;
            CGuiListBoxBase_EnsureColumn(
                self, nullptr, static_cast<std::int32_t>(columnIndex));

            if (self->m_columnsBegin)
            {
                const auto currentCount = static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(
                        self->m_columnsEnd - self->m_columnsBegin) / 12);
                if (columnIndex < currentCount)
                {
                    CSF_AT(std::int32_t,
                        reinterpret_cast<std::uint8_t*>(self->m_columnsBegin) + columnOffset,
                        0) = scaledWidth;
                }
            }
        }

        ++columnIndex;
        columnOffset += 12;
    }

    CGuiListBoxBase_PrepareVisibleRows(self, nullptr);

    void* size = self->vGetSize();
    const std::uint32_t rowHeight = self->m_rowHeight;
    const std::uint32_t width = CSF_AT(std::uint32_t, size, 0);
    self->field_01c8 = rowHeight;
    auto* slider = reinterpret_cast<CGuiControl*>(self->m_pSlider);
    self->field_01c4 = width;
    if (slider)
        slider->vSetPosition_54(scaleX, scaleY);

    auto* header = reinterpret_cast<CGuiControl*>(self->m_pHeaderText);
    if (header)
    {
        float scale[2] = {scaleX, scaleY};
        header->vSetPosition(reinterpret_cast<const std::int32_t*>(scale));
    }

    columnIndex = 0;
    columnOffset = 0;
    for (;;)
    {
        std::uint32_t result = self->m_columnsBegin;
        if (!result)
            return result;

        result = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->m_columnsEnd - result) / 12);
        if (columnIndex >= result)
            return result;

        auto* column = reinterpret_cast<std::uint8_t*>(self->m_columnsBegin) + columnOffset;
        auto* text = reinterpret_cast<CGuiControl*>(CSF_AT(std::uint32_t, column, 4));
        if (text)
        {
            float scale[2] = {scaleX, scaleY};
            text->vSetPosition(reinterpret_cast<const std::int32_t*>(scale));
        }

        ++columnIndex;
        columnOffset += 12;
    }
}

HOOK(0x00717D90, CGuiListBoxBase_ScaleLayout, "auto");
