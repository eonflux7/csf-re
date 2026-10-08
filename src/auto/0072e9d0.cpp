#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto CGuiSlider_IsSelectableAndNotType10 =
    reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(CSF_ADDR_FN(0x00718700));

static const auto CGuiButton_HandleInputEvent =
    reinterpret_cast<char (__fastcall*)(CGuiButton*, void*, int, int)>(CSF_ADDR_FN(0x007259A0));

static char __fastcall CGuiChoiceButton_HandleInputEvent(
    CGuiChoiceButton* self, void* /*edx*/, int event, int argument)
{
    char result = static_cast<char>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        CGuiSlider_IsSelectableAndNotType10(self, nullptr));
    if (!result)
        return result;

    if (event == 5)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x2BC / 4])(self, nullptr, 1);
    }

    return CGuiButton_HandleInputEvent(self, nullptr, event, argument);
}

HOOK(0x0072E9D0, CGuiChoiceButton_HandleInputEvent, "auto");
