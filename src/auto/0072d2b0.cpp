#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiSlider_GetPositionFromCoordinate(
    CGuiSlider* self, void* /*edx*/, int coordinate)
{
    if (!self->m_pArrowUp || !self->m_pArrowDown || !self->m_count)
        return 0;

    int32_t trackStart;
    int32_t trackEnd;

    if (!self->m_bHorizontal &&
        CSF_AT(int32_t, self->vGetSize(), 0x04) != 0)
    {
        auto* arrowUp = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
        char* position = arrowUp->vGetEmbeddedA8();
        void* size = arrowUp->vGetSize();
        int32_t height = CSF_AT(int32_t, size, 0x04);
        int32_t top = CSF_AT(int32_t, position, 0x04);
        trackStart = static_cast<int32_t>(
            static_cast<uint32_t>(top) + static_cast<uint32_t>(height));

        char* endPosition =
            reinterpret_cast<CGuiControl*>(self->m_pArrowDown)->vGetEmbeddedA8();
        trackEnd = CSF_AT(int32_t, endPosition, 0x04);
    }
    else
    {
        if (self->m_bHorizontal != 1)
            return 0;
        if (CSF_AT(int32_t, self->vGetSize(), 0x00) == 0)
            return 0;

        void* size =
            reinterpret_cast<CGuiControl*>(self->m_pArrowUp)->vGetSize();
        char* position =
            reinterpret_cast<CGuiControl*>(self->m_pArrowUp)->vGetEmbeddedA8();
        int32_t left = CSF_AT(int32_t, position, 0x00);
        int32_t width = CSF_AT(int32_t, size, 0x00);
        trackStart = static_cast<int32_t>(
            static_cast<uint32_t>(width) + static_cast<uint32_t>(left));

        char* endPosition =
            reinterpret_cast<CGuiControl*>(self->m_pArrowDown)->vGetEmbeddedA8();
        trackEnd = CSF_AT(int32_t, endPosition, 0x00);
    }

    int32_t displacement = static_cast<int32_t>(
        static_cast<uint32_t>(coordinate) - static_cast<uint32_t>(trackStart));
    if (displacement <= 0)
        return 0;
    if (displacement >= trackEnd)
        return static_cast<int32_t>(self->m_count - 1);

    int32_t numerator = static_cast<int32_t>(
        self->m_count * static_cast<uint32_t>(displacement));
    int32_t denominator = static_cast<int32_t>(
        static_cast<uint32_t>(trackEnd) - static_cast<uint32_t>(trackStart));

    return static_cast<int32_t>(
        static_cast<int64_t>(static_cast<double>(numerator) /
                             static_cast<double>(denominator)));
}

HOOK(0x0072D2B0, CGuiSlider_GetPositionFromCoordinate, "auto");
