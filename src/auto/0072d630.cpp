#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiSlider_SetSelectedPart(
    CGuiSlider* self, void* /*edx*/, CGuiControl* selectedPart)
{
    auto* previousPart =
        reinterpret_cast<CGuiControl*>(self->m_pSelectedPart);

    if (previousPart != selectedPart)
    {
        if (previousPart)
        {
            reinterpret_cast<void(__fastcall*)(CGuiControl*, void*, int)>(
                CSF_AT(void**, previousPart, 0)[0xB8 / 4])(
                    previousPart, nullptr, 0);
        }

        self->m_pSelectedPart =
            reinterpret_cast<std::uint32_t>(selectedPart);

        if (selectedPart)
        {
            reinterpret_cast<void(__fastcall*)(CGuiControl*, void*, int)>(
                CSF_AT(void**, selectedPart, 0)[0xB8 / 4])(
                    selectedPart, nullptr, 1);
        }
    }

    return CGuiSlider_ForwardSelfToParent(self, nullptr);
}

HOOK(0x0072D630, CGuiSlider_SetSelectedPart, "auto");
