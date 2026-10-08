#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10,
         0x00718700, (uint8_t* self, void* edx));

CSF_FUNC(char, __fastcall, CGuiButton_HandleSelectionInput,
         0x007259A0, (CGuiButton* self, void* edx, int event, int argument));

static char __fastcall CGuiChoiceNumButton_HandleEvent(
    CGuiChoiceNumButton* self, void* /*edx*/, int event, int argument)
{
    char result = static_cast<char>(
        (reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4]))(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        (reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4]))(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(CGuiSlider_IsSelectableAndNotType10(
        reinterpret_cast<uint8_t*>(self), nullptr));
    if (!result)
        return result;

    if (event == 5)
    {
        const uint32_t value = self->field_0200;
        (reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, self, 0)[0x2B0 / 4]))(self, nullptr, value);
    }

    return CGuiButton_HandleSelectionInput(self, nullptr, event, argument);
}

HOOK(0x0072F9D0, CGuiChoiceNumButton_HandleEvent, "auto");
