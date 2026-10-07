#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall CGuiButton_GetStateQuad(
    CGuiButton* self, void* /*edx*/)
{
    const std::uint32_t state = self->field_01e8;

    if (state == 1)
        return reinterpret_cast<std::uint32_t*>(self->m_pIdleQuad);

    if (state == 0)
    {
        auto* hoverQuad =
            reinterpret_cast<std::uint32_t*>(self->m_pHoverQuad);
        if (hoverQuad)
            return hoverQuad;

        return reinterpret_cast<std::uint32_t*>(self->m_pIdleQuad);
    }

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00725090, CGuiButton_GetStateQuad, "auto");
