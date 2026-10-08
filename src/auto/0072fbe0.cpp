#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiSlider_GetField0148, 0x00730110,
         (CGuiRadioButton* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiButton_ResetEffects, 0x00726090,
         (CGuiRadioButton* self, void* edx, char selected));
CSF_FUNC(void, __fastcall, CGuiButton_StartTextColorEffect, 0x00725740,
         (CGuiRadioButton* self, void* edx));

static void __fastcall CGuiRadioButton_SetSelected(
    CGuiRadioButton* self, void* /*edx*/, char selected)
{
    const auto currentSelection =
        reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(selected) == currentSelection)
        return;

    std::uint8_t newSelection;
    if (self->m_bSelectable || !selected)
        newSelection = static_cast<std::uint8_t>(selected);
    else
        newSelection = self->m_bSelected;

    self->m_bSelected = newSelection;

    if (CGuiSlider_GetField0148(self, nullptr) != 1)
        CGuiButton_ResetEffects(self, nullptr, selected);

    if (selected)
    {
        CGuiButton_StartTextColorEffect(self, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
    }
}

HOOK(0x0072FBE0, CGuiRadioButton_SetSelected, "auto");
