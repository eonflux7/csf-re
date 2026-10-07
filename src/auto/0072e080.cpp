#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiSlider_UpdateConsoleArrowSelectability,
         0x0072CEF0, (CGuiSlider* self, void* edx));
CSF_FUNC(int32_t, __fastcall, CGuiSlider_GetThumbCoordinate,
         0x0072D400, (CGuiSlider* self, void* edx, int32_t position));

static char __fastcall CGuiSlider_SetPosition(
    CGuiSlider* self, void* /*edx*/, int32_t position, char allowWrap)
{
    int32_t newPosition = position;

    if (allowWrap && self->m_bWrap)
    {
        const int32_t wrapCount = static_cast<int32_t>(self->m_count);
        if (newPosition >= wrapCount)
            newPosition = 0;
        if (newPosition < 0)
            newPosition = static_cast<int32_t>(
                static_cast<uint32_t>(wrapCount) - 1u);
    }

    const int32_t count = static_cast<int32_t>(self->m_count);
    if (newPosition >= count || newPosition < 0)
        return static_cast<char>(newPosition);

    const int32_t pageSize = static_cast<int32_t>(self->m_pageSize);
    if (count > pageSize)
    {
        const int32_t maximumPosition = static_cast<int32_t>(
            static_cast<uint32_t>(count) - static_cast<uint32_t>(pageSize));
        if (newPosition > maximumPosition)
            newPosition = maximumPosition;
    }

    void* thumb = reinterpret_cast<void*>(self->m_pThumb);
    self->m_pos = static_cast<uint32_t>(newPosition);
    self->field_0138 = static_cast<float>(newPosition);

    if (thumb)
    {
        const int32_t* currentPosition =
            (reinterpret_cast<int32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, thumb, 0)[0x9C / 4]))(thumb, nullptr);

        int32_t thumbPosition[2];
        thumbPosition[0] = currentPosition[0];
        thumbPosition[1] = currentPosition[1];

        const uint32_t orientation = self->m_bHorizontal;
        if (orientation == 0)
        {
            thumbPosition[1] = CGuiSlider_GetThumbCoordinate(
                self, nullptr, static_cast<int32_t>(self->m_pos));
        }
        else if (orientation == 1)
        {
            thumbPosition[0] = CGuiSlider_GetThumbCoordinate(
                self, nullptr, static_cast<int32_t>(self->m_pos));
        }

        thumb = reinterpret_cast<void*>(self->m_pThumb);
        (reinterpret_cast<void (__fastcall*)(void*, void*, int32_t*)>(
            CSF_AT(void**, thumb, 0)[0x98 / 4]))(
                thumb, nullptr, thumbPosition);
    }

    return CGuiSlider_UpdateConsoleArrowSelectability(self, nullptr);
}

HOOK(0x0072E080, CGuiSlider_SetPosition, "auto");
