#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// This CRT entry takes its operand in ST(0), not as a stack argument.
CSF_FUNC(std::int64_t, __cdecl, TruncateX87ToInteger, 0x007AF86C, ());

static const std::uint32_t* GetControlPosition(CGuiControl* control)
{
    return reinterpret_cast<
        const std::uint32_t* (__fastcall*)(void*, void*)
    >(CSF_AT(void**, control, 0)[0x9C / 4])(control, nullptr);
}

static const std::uint32_t* GetControlSize(CGuiControl* control)
{
    return reinterpret_cast<
        const std::uint32_t* (__fastcall*)(void*, void*)
    >(CSF_AT(void**, control, 0)[0xA4 / 4])(control, nullptr);
}

static std::uint32_t ScaleSliderTrackDistance(
    std::uint32_t distance, int position, std::uint32_t count)
{
    const std::uint32_t product =
        distance * static_cast<std::uint32_t>(position);

    const double quotient =
        static_cast<double>(static_cast<std::int32_t>(product)) /
        static_cast<double>(static_cast<std::int32_t>(count));

    // Reproduce __ftol2's low DWORD numerically. Its ST(0)-input ABI
    // cannot be invoked through an ordinary C++ function pointer.
    (void)TruncateX87ToInteger;
    if (quotient >= -2147483648.0 && quotient <= 2147483648.0)
        return static_cast<std::uint32_t>(
            static_cast<std::int64_t>(quotient));

    // A zero denominator produces an indefinite integer with low DWORD 0.
    return 0;
}

static int __fastcall CGuiSlider_GetThumbCoordinate(
    CGuiSlider* self, void* /*edx*/, int position)
{
    if (!self->m_pArrowUp || !self->m_pArrowDown || !self->m_count)
        return 0;

    std::uint32_t trackStart;
    std::uint32_t trackEnd;
    unsigned axis;

    if (self->m_bHorizontal == 0)
    {
        axis = 1;
        auto* arrowUp =
            reinterpret_cast<CGuiControl*>(self->m_pArrowUp);

        const auto* arrowPosition = GetControlPosition(arrowUp);
        const auto* arrowSize = GetControlSize(arrowUp);

        const std::uint32_t coordinate = arrowPosition[1];
        const std::uint32_t extent = arrowSize[1];
        trackStart = coordinate + extent;

        const auto* downPosition = GetControlPosition(
            reinterpret_cast<CGuiControl*>(self->m_pArrowDown));
        trackEnd = downPosition[1];
    }
    else if (self->m_bHorizontal == 1)
    {
        axis = 0;
        const auto* arrowSize = GetControlSize(
            reinterpret_cast<CGuiControl*>(self->m_pArrowUp));
        const auto* arrowPosition = GetControlPosition(
            reinterpret_cast<CGuiControl*>(self->m_pArrowUp));

        const std::uint32_t coordinate = arrowPosition[0];
        const std::uint32_t extent = arrowSize[0];
        trackStart = coordinate + extent;

        const auto* downPosition = GetControlPosition(
            reinterpret_cast<CGuiControl*>(self->m_pArrowDown));
        trackEnd = downPosition[0];
    }
    else
    {
        return 0;
    }

    const std::uint32_t coordinate = trackStart +
        ScaleSliderTrackDistance(
            trackEnd - trackStart, position, self->m_count);

    const auto* thumbSize = GetControlSize(
        reinterpret_cast<CGuiControl*>(self->m_pThumb));

    if (static_cast<std::int32_t>(coordinate + thumbSize[axis]) >
        static_cast<std::int32_t>(trackEnd))
    {
        const auto* updatedThumbSize = GetControlSize(
            reinterpret_cast<CGuiControl*>(self->m_pThumb));
        return static_cast<std::int32_t>(
            trackEnd - updatedThumbSize[axis]);
    }

    return static_cast<std::int32_t>(coordinate);
}

HOOK(0x0072D400, CGuiSlider_GetThumbCoordinate, "auto");
