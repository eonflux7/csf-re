#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets the slider's "down" arrow part (+0x148) and marks it as the currently
// selected part (+0x154). Any previously installed arrow is released through
// its own virtual slot at vtable[0] (called with argument 1).
static int __fastcall CGuiSlider_SetDownArrow(
    CGuiSlider* self, void* /*edx*/, CGuiControl* downArrow)
{
    int result;
    auto* previousArrow =
        reinterpret_cast<CGuiControl*>(self->m_pArrowDown);

    if (previousArrow)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, previousArrow, 0)[0])(
                previousArrow, nullptr, 1);
        self->m_pArrowDown = 0;
    }

    self->m_pArrowDown = reinterpret_cast<std::uintptr_t>(downArrow);
    self->m_pSelectedPart = reinterpret_cast<std::uintptr_t>(downArrow);

    if (downArrow)
    {
        // vtable slot +0x108 -> let the part know it belongs to this slider.
        return ((int(__fastcall*)(void*, void*, CGuiSlider*))
            CSF_AT(void**, downArrow, 0)[0x108 / 4])(
                downArrow, nullptr, self);
    }

    return result;
}

HOOK(0x0072CCC0, CGuiSlider_SetDownArrow, "auto");
