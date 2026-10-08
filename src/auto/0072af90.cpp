#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_HandleSelectionInput_718300, 0x00718300,
         (CGuiControl* self, void* edx, int input));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));

static char __fastcall CGuiPanel_ProcessChildrenAndInput(
    CGuiPanel* self, void* /*edx*/, int input)
{
    char result = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        CGuiSlider_IsSelectableAndNotType10(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0xC4 / 4])(self, nullptr));
    if (!result)
        return result;

    const std::uint32_t childrenBegin = self->field_0138;
    const std::uint32_t childCount = childrenBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_013c - childrenBegin) >> 2)
        : 0;

    for (std::uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
    {
        CGuiControl* child =
            reinterpret_cast<CGuiControl**>(self->field_0138)[childIndex];

        const int previousState =
            ((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x140 / 4])(child, nullptr);

        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, child, 0)[0x14 / 4])(child, nullptr, input);

        const int childFlag =
            ((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x60 / 4])(child, nullptr);

        if (!childFlag)
        {
            const int currentState =
                ((int(__fastcall*)(void*, void*))
                     CSF_AT(void**, child, 0)[0x140 / 4])(child, nullptr);
            if (currentState != previousState)
                break;
        }
    }

    return CGuiControl_HandleSelectionInput_718300(self, nullptr, input);
}

HOOK(0x0072AF90, CGuiPanel_ProcessChildrenAndInput, "auto");
