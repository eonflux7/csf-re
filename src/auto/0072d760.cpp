#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));

static char __fastcall CGuiSlider_RedirectConsoleArrowSelection(
    CGuiSlider* self, void* /*edx*/)
{
    std::uint8_t result = self->m_bConsole;
    if (!result)
        return static_cast<char>(result);

    auto* selectedPart =
        ((CGuiControl* (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x104 / 4])(self, nullptr);

    result = static_cast<std::uint8_t>(
        reinterpret_cast<std::uintptr_t>(selectedPart));
    if (!selectedPart)
        return static_cast<char>(result);

    if (selectedPart == reinterpret_cast<CGuiControl*>(self->m_pArrowUp))
    {
        result = static_cast<std::uint8_t>(
            CGuiSlider_IsSelectableAndNotType10(selectedPart, nullptr));
        if (!result)
        {
            auto* arrowDown =
                reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
            return static_cast<char>(
                ((int (__fastcall*)(void*, void*, CGuiControl*))
                    CSF_AT(void**, self, 0)[0x158 / 4])(
                        self, nullptr, arrowDown));
        }
    }

    if (selectedPart == reinterpret_cast<CGuiControl*>(self->m_pArrowDown))
    {
        result = static_cast<std::uint8_t>(
            CGuiSlider_IsSelectableAndNotType10(selectedPart, nullptr));
        if (!result)
        {
            auto* arrowUp =
                reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
            return static_cast<char>(
                ((int (__fastcall*)(void*, void*, CGuiControl*))
                    CSF_AT(void**, self, 0)[0x158 / 4])(
                        self, nullptr, arrowUp));
        }
    }

    return static_cast<char>(result);
}

HOOK(0x0072D760, CGuiSlider_RedirectConsoleArrowSelection, "auto");
