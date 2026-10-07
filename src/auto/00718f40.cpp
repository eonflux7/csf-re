#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall CGuiControl_SetAbsolutePosition(
    CGuiControl* self, void* /*edx*/, std::uint32_t* position)
{
    auto* parent = reinterpret_cast<CGuiControl*>(self->m_pParent);
    if (parent)
    {
        std::uint32_t parentPosition[2];
        auto* result =
            (reinterpret_cast<std::uint32_t* (__fastcall*)(
                CGuiControl*, void*, std::uint32_t*)>(
                parent->vtbl[0x160 / 4]))(
                    parent, nullptr, parentPosition);

        std::uint32_t x = position[0];
        const std::uint32_t parentX = result[0];
        std::uint32_t y = position[1];
        x -= parentX;
        y -= result[1];

        self->m_x = x;
        self->m_y = y;
        return result;
    }

    self->m_x = position[0];
    self->m_y = position[1];
    return position;
}

HOOK(0x00718F40, CGuiControl_SetAbsolutePosition, "auto");
