#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiSlider_UpdateConsoleArrowSelectability,
         0x0072CEF0, (CGuiSlider* self, void* edx));

// This runtime helper takes its input in ST(0), not on the C++ stack.
// Its address is retained; the conversion below reproduces its integer result.
CSF_FUNC(std::int64_t, __cdecl, TruncateX87ToInt64,
         0x007AF86C, ());

static std::uint32_t TruncateToLowWord(double value)
{
    // An invalid x87 integer conversion produces INT64_MIN, whose low word is zero.
    if (!(value >= -9223372036854775808.0 &&
          value <   9223372036854775808.0))
        return 0;

    return static_cast<std::uint32_t>(static_cast<std::int64_t>(value));
}

static std::uint32_t* GetControlPosition(CGuiControl* control)
{
    return reinterpret_cast<std::uint32_t* (__fastcall*)(
        CGuiControl*, void*)>(CSF_AT(void**, control, 0)[0x9C / 4])(
            control, nullptr);
}

static std::uint32_t* GetControlSize(CGuiControl* control)
{
    return reinterpret_cast<std::uint32_t* (__fastcall*)(
        CGuiControl*, void*)>(CSF_AT(void**, control, 0)[0xA4 / 4])(
            control, nullptr);
}

static void SetControlSize(CGuiControl* control, std::uint32_t* size)
{
    reinterpret_cast<void (__fastcall*)(
        CGuiControl*, void*, std::uint32_t*)>(
            CSF_AT(void**, control, 0)[0xA0 / 4])(
                control, nullptr, size);
}

static char __fastcall CGuiSlider_SetRange(
    CGuiSlider* self, void* /*edx*/,
    std::int32_t count, std::int32_t pageSize, char resizeThumb)
{
    self->m_count = static_cast<std::uint32_t>(count);
    self->m_pageSize = static_cast<std::uint32_t>(pageSize);

    if (resizeThumb && self->m_pThumb)
    {
        auto* thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
        const std::uint32_t orientation = self->m_bHorizontal;

        if (orientation == 1)
        {
            std::uint32_t* currentSize = GetControlSize(thumb);
            std::uint32_t thumbSize[2];
            thumbSize[0] = currentSize[0];
            thumbSize[1] = currentSize[1];

            std::uint32_t* downPosition = GetControlPosition(
                reinterpret_cast<CGuiControl*>(self->m_pArrowDown));
            std::uint32_t* upPosition = GetControlPosition(
                reinterpret_cast<CGuiControl*>(self->m_pArrowUp));
            std::uint32_t* upSize = GetControlSize(
                reinterpret_cast<CGuiControl*>(self->m_pArrowUp));

            const std::uint32_t upX = upPosition[0];
            const std::uint32_t downX = downPosition[0];
            const std::uint32_t upWidth = upSize[0];
            const std::uint32_t available = downX - upX - upWidth;

            const double scaledSize =
                static_cast<double>(static_cast<std::int32_t>(available)) /
                static_cast<double>(static_cast<std::int32_t>(self->m_count)) *
                static_cast<double>(static_cast<std::int32_t>(self->m_pageSize));
            thumbSize[0] = TruncateToLowWord(scaledSize);

            SetControlSize(
                reinterpret_cast<CGuiControl*>(self->m_pThumb), thumbSize);
        }
        else if (orientation == 0)
        {
            std::uint32_t* currentSize = GetControlSize(thumb);
            std::uint32_t thumbSize[2];
            thumbSize[0] = currentSize[0];
            auto* down = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
            thumbSize[1] = currentSize[1];
            auto* up = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);

            std::uint32_t* upPosition = GetControlPosition(up);
            std::uint32_t* downPosition = GetControlPosition(down);
            const std::uint32_t downY = downPosition[1];
            const std::uint32_t upY = upPosition[1];
            std::uint32_t available = downY - upY;

            std::uint32_t* upSize = GetControlSize(up);
            available -= upSize[1];

            const double scaledSize =
                static_cast<double>(static_cast<std::int32_t>(available)) /
                static_cast<double>(static_cast<std::int32_t>(self->m_count)) *
                static_cast<double>(static_cast<std::int32_t>(self->m_pageSize));
            thumbSize[1] = TruncateToLowWord(scaledSize);

            SetControlSize(
                reinterpret_cast<CGuiControl*>(self->m_pThumb), thumbSize);
        }
    }

    const std::uint32_t currentCount = self->m_count;
    if (static_cast<std::int32_t>(self->m_pos) >=
        static_cast<std::int32_t>(currentCount))
    {
        const std::uint32_t lastPosition = currentCount - 1u;
        self->m_pos = lastPosition;
        self->field_0138 =
            static_cast<float>(static_cast<std::int32_t>(lastPosition));
    }

    if (static_cast<std::int32_t>(self->m_pos) < 0)
    {
        self->m_pos = 0;
        self->field_0138 = 0.0f;
    }

    return CGuiSlider_UpdateConsoleArrowSelectability(self, nullptr);
}

HOOK(0x0072D120, CGuiSlider_SetRange, "auto");
