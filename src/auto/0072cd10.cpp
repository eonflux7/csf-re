#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiSlider_GetTrackLength(CGuiSlider* self, void* /*edx*/)
{
    const std::uint32_t horizontal = self->m_bHorizontal;
    auto* arrowUp = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);

    if (horizontal != 0)
    {
        auto* size = reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, arrowUp, 0)[0xA4 / 4])(arrowUp, nullptr);

        arrowUp = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
        auto* position = reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, arrowUp, 0)[0x9C / 4])(arrowUp, nullptr);

        const std::uint32_t startX = CSF_AT(std::uint32_t, position, 0);
        const std::uint32_t width = CSF_AT(std::uint32_t, size, 0);
        auto* arrowDown = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
        const std::uint32_t trackStart = width + startX;

        auto* endPosition = reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, arrowDown, 0)[0x9C / 4])(arrowDown, nullptr);

        return static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, endPosition, 0) - trackStart);
    }

    auto* position = reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, arrowUp, 0)[0x9C / 4])(arrowUp, nullptr);

    auto* size = reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, arrowUp, 0)[0xA4 / 4])(arrowUp, nullptr);

    const std::uint32_t height = CSF_AT(std::uint32_t, size, 4);
    auto* arrowDown = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
    const std::uint32_t startY = CSF_AT(std::uint32_t, position, 4);
    const std::uint32_t trackStart = startY + height;

    auto* endPosition = reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, arrowDown, 0)[0x9C / 4])(arrowDown, nullptr);

    return static_cast<std::int32_t>(
        CSF_AT(std::uint32_t, endPosition, 4) - trackStart);
}

HOOK(0x0072CD10, CGuiSlider_GetTrackLength, "auto");
