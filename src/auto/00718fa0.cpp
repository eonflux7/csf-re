#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall CGuiControl_GetParentPosition(
    CGuiControl* self, void* /*edx*/, std::uint32_t* position)
{
    auto* parent = reinterpret_cast<CGuiControl*>(self->m_pParent);
    if (parent)
    {
        using GetPosition = void(__fastcall*)(
            CGuiControl*, void*, std::uint32_t*);
        reinterpret_cast<GetPosition>(parent->vtbl[0x160 / 4])(
            parent, nullptr, position);
    }
    else
    {
        position[0] = 0;
        position[1] = 0;
    }
    return position;
}

HOOK(0x00718FA0, CGuiControl_GetParentPosition, "auto");
