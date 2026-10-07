#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiSlider_SetUpArrow(
    CGuiSlider* self, void* /*edx*/, CGuiControl* upArrow)
{
    int result;
    auto* previousArrow =
        reinterpret_cast<CGuiControl*>(self->m_pArrowUp);

    if (previousArrow)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, previousArrow, 0)[0])(
                previousArrow, nullptr, 1);
        self->m_pArrowUp = 0;
    }

    self->m_pArrowUp = reinterpret_cast<std::uintptr_t>(upArrow);
    self->m_pSelectedPart = reinterpret_cast<std::uintptr_t>(upArrow);

    if (upArrow)
    {
        return ((int(__fastcall*)(void*, void*, CGuiSlider*))
            CSF_AT(void**, upArrow, 0)[0x108 / 4])(
                upArrow, nullptr, self);
    }

    return result;
}

HOOK(0x0072CC70, CGuiSlider_SetUpArrow, "auto");
