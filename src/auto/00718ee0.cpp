#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int* __fastcall CGuiControl_GetAbsolutePosition(
    CGuiControl* self, void* /*edx*/, int* position)
{
    auto* parent = reinterpret_cast<CGuiControl*>(self->m_pParent);
    if (parent)
    {
        int parentPosition[2];
        int* absoluteParentPosition =
            ((int* (__fastcall*)(void*, void*, int*))
                parent->vtbl[0x160 / 4])(parent, nullptr, parentPosition);

        const uint32_t y =
            static_cast<uint32_t>(absoluteParentPosition[1]) + self->m_y;
        const uint32_t x =
            static_cast<uint32_t>(absoluteParentPosition[0]) + self->m_x;

        position[0] = static_cast<int>(x);
        position[1] = static_cast<int>(y);
    }
    else
    {
        position[0] = static_cast<int>(self->m_x);
        position[1] = static_cast<int>(self->m_y);
    }
    return position;
}

HOOK(0x00718EE0, CGuiControl_GetAbsolutePosition, "auto");
