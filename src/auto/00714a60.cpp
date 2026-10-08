#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiListBoxBase* self, void* edx));

static int __fastcall CGuiListBoxBase_RenderVisibleRowHighlights(
    CGuiListBoxBase* self, void* /*edx*/)
{
    int result = static_cast<int32_t>(self->m_columnsBegin);
    if (result == 0)
        return result;

    result = static_cast<int32_t>(
        self->m_columnsEnd - self->m_columnsBegin) / 12;
    if (result == 0)
        return result;

    result = CGuiSlider_IsSelectableAndNotType10(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    uint32_t origin[2];
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr, origin);

    origin[1] += self->field_0174 + self->field_0178;

    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

    const uint32_t availableHeight =
        CSF_AT(uint32_t, dimensions, 4)
        - self->field_0174 - self->field_0178;
    const int32_t rowStride = static_cast<int32_t>(
        self->m_rowHeight + self->m_lineSpacing * 2u);
    const int32_t remainder =
        static_cast<int32_t>(availableHeight) % rowStride;
    const uint8_t highlightsEnabled = self->field_0140;
    const int32_t visibleRows = static_cast<int32_t>(
        availableHeight - static_cast<uint32_t>(remainder)) / rowStride;

    result = static_cast<int32_t>(
        static_cast<uint32_t>(visibleRows) + self->field_0168);
    if (highlightsEnabled == 0)
        return result;

    const uint32_t indicesBegin = self->field_01ac;
    const uint32_t indexCount = indicesBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_01b0 - indicesBegin) >> 2)
        : 0;

    for (uint32_t index = 0; index < indexCount; ++index)
    {
        auto* rowIndices =
            reinterpret_cast<int32_t*>(self->field_01ac);
        const int32_t firstVisibleRow =
            static_cast<int32_t>(self->field_0168);

        if (rowIndices[index] < firstVisibleRow)
            continue;
        if (rowIndices[index] >= result)
            continue;

        uint32_t position[2] = {origin[0], origin[1]};
        const uint32_t highlightStride =
            self->m_rowHeight + self->m_lineSpacing * 2u;
        const uint32_t rowOffset =
            (static_cast<uint32_t>(rowIndices[index]) - self->field_0168)
            * highlightStride + self->field_01c0;

        position[0] += self->field_01bc;
        position[1] += rowOffset;

        void* highlight = reinterpret_cast<void*>(self->field_013c);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, highlight, 0)[0x1C / 4])(
                highlight, nullptr, position);

        highlight = reinterpret_cast<void*>(self->field_013c);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, highlight, 0)[0x24 / 4])(
                highlight, nullptr, &self->field_01c4);

        const uint8_t focused =
            reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);

        float* color = focused ? self->m_itemColor : self->m_noFocusColor;
        highlight = reinterpret_cast<void*>(self->field_013c);
        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, highlight, 0)[0x2C / 4])(
                highlight, nullptr, color);

        highlight = reinterpret_cast<void*>(self->field_013c);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, self, 0)[0x238 / 4])(
                self, nullptr, highlight);
    }

    return result;
}

HOOK(0x00714A60, CGuiListBoxBase_RenderVisibleRowHighlights, "auto");
