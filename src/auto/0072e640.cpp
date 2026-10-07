#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_UpdateMouseHover, 0x00718DE0,
         (CGuiControl* self, void* edx, std::int32_t* mousePosition, char flags));

static int __fastcall CGuiSlider_UpdateMouseHover(
    CGuiSlider* self, void* /*edx*/,
    std::int32_t* mousePosition, int flags)
{
    using GetGlobalPosition = std::uint32_t* (__fastcall*)(
        void*, void*, std::uint32_t*);
    using UpdateMouseHover = void (__fastcall*)(
        void*, void*, std::uint32_t*, int);

    std::uint32_t positionBufferA[2];
    std::uint32_t positionBufferB[2];
    std::uint32_t childMousePosition[2];

    if (self->m_pArrowUp)
    {
        const auto sliderPosition =
            reinterpret_cast<GetGlobalPosition>(
                CSF_AT(void**, self, 0)[0x160 / 4])(
                    self, nullptr, positionBufferB);

        const std::uint32_t sliderX = sliderPosition[0];
        const std::uint32_t sliderY = sliderPosition[1];
        const std::uint32_t mouseY =
            static_cast<std::uint32_t>(mousePosition[1]);
        const std::uint32_t mouseX =
            static_cast<std::uint32_t>(mousePosition[0]);
        const std::uint32_t globalY = sliderY + mouseY;
        const std::uint32_t globalX = sliderX + mouseX;

        auto child = reinterpret_cast<void*>(self->m_pArrowUp);
        const auto childPosition =
            reinterpret_cast<GetGlobalPosition>(
                CSF_AT(void**, child, 0)[0x160 / 4])(
                    child, nullptr, positionBufferA);

        const std::uint32_t childY = childPosition[1];
        const std::uint32_t childX = childPosition[0];
        childMousePosition[1] = globalY - childY;
        childMousePosition[0] = globalX - childX;

        child = reinterpret_cast<void*>(self->m_pArrowUp);
        reinterpret_cast<UpdateMouseHover>(
            CSF_AT(void**, child, 0)[0x14C / 4])(
                child, nullptr, childMousePosition, flags);
    }

    if (self->m_pArrowDown)
    {
        const auto sliderPosition =
            reinterpret_cast<GetGlobalPosition>(
                CSF_AT(void**, self, 0)[0x160 / 4])(
                    self, nullptr, positionBufferA);

        const std::uint32_t sliderX = sliderPosition[0];
        const std::uint32_t sliderY = sliderPosition[1];
        const std::uint32_t mouseY =
            static_cast<std::uint32_t>(mousePosition[1]);
        const std::uint32_t mouseX =
            static_cast<std::uint32_t>(mousePosition[0]);
        const std::uint32_t globalY = sliderY + mouseY;
        const std::uint32_t globalX = sliderX + mouseX;

        auto child = reinterpret_cast<void*>(self->m_pArrowDown);
        const auto childPosition =
            reinterpret_cast<GetGlobalPosition>(
                CSF_AT(void**, child, 0)[0x160 / 4])(
                    child, nullptr, positionBufferB);

        const std::uint32_t childY = childPosition[1];
        const std::uint32_t childX = childPosition[0];
        childMousePosition[1] = globalY - childY;
        childMousePosition[0] = globalX - childX;

        child = reinterpret_cast<void*>(self->m_pArrowDown);
        reinterpret_cast<UpdateMouseHover>(
            CSF_AT(void**, child, 0)[0x14C / 4])(
                child, nullptr, childMousePosition, flags);
    }

    if (self->m_pThumb)
    {
        const auto sliderPosition =
            reinterpret_cast<GetGlobalPosition>(
                CSF_AT(void**, self, 0)[0x160 / 4])(
                    self, nullptr, positionBufferA);

        const std::uint32_t sliderX = sliderPosition[0];
        const std::uint32_t sliderY = sliderPosition[1];
        const std::uint32_t mouseY =
            static_cast<std::uint32_t>(mousePosition[1]);
        const std::uint32_t mouseX =
            static_cast<std::uint32_t>(mousePosition[0]);
        const std::uint32_t globalY = sliderY + mouseY;
        const std::uint32_t globalX = sliderX + mouseX;

        auto child = reinterpret_cast<void*>(self->m_pThumb);
        const auto childPosition =
            reinterpret_cast<GetGlobalPosition>(
                CSF_AT(void**, child, 0)[0x160 / 4])(
                    child, nullptr, positionBufferB);

        const std::uint32_t childY = childPosition[1];
        const std::uint32_t childX = childPosition[0];
        childMousePosition[1] = globalY - childY;
        childMousePosition[0] = globalX - childX;

        child = reinterpret_cast<void*>(self->m_pThumb);
        reinterpret_cast<UpdateMouseHover>(
            CSF_AT(void**, child, 0)[0x14C / 4])(
                child, nullptr, childMousePosition, flags);
    }

    return CGuiControl_UpdateMouseHover(
        self, nullptr, mousePosition, static_cast<char>(flags));
}

HOOK(0x0072E640, CGuiSlider_UpdateMouseHover, "auto");
