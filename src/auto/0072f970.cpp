#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10,
         0x00718700, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiButton_HandleInput,
         0x00726020, (void* self, void* edx, int inputCode, int inputData));

static char __fastcall CGuiChoiceNumButton_HandleInput(
    CGuiChoiceNumButton* self, void* /*edx*/, int inputCode, int inputData)
{
    char result = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
         CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
         CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        CGuiSlider_IsSelectableAndNotType10(self, nullptr));
    if (!result)
        return result;

    if (inputCode == 28 || inputCode == 156)
    {
        const std::uint32_t activationValue = self->field_0200;
        void** vtable = CSF_AT(void**, self, 0);

        ((void(__fastcall*)(void*, void*, std::uint32_t))
         vtable[0x2B0 / 4])(self, nullptr, activationValue);
    }

    return CGuiButton_HandleInput(self, nullptr, inputCode, inputData);
}

HOOK(0x0072F970, CGuiChoiceNumButton_HandleInput, "auto");
