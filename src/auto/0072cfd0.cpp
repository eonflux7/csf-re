#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetSelectable, 0x007186D0,
         (void* self, void* edx, char selectable));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (void* self, void* edx));

static char __fastcall RefreshChildControlStates(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x16A);

    if (result != 0)
    {
        if (CSF_AT(void*, self, 0x144) != nullptr)
        {
            const int position =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x270 / 4])(self, nullptr);

            CGuiControl_SetSelectable(
                CSF_AT(void*, self, 0x144), nullptr,
                static_cast<char>(position > 0));
        }

        result = 0;
        if (CSF_AT(void*, self, 0x148) != nullptr)
        {
            const int span =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x278 / 4])(self, nullptr);

            const int position =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x270 / 4])(self, nullptr);

            const std::int32_t endPosition = static_cast<std::int32_t>(
                static_cast<std::uint32_t>(span) +
                static_cast<std::uint32_t>(position));

            const int limit =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x264 / 4])(self, nullptr);

            result = static_cast<char>(CGuiControl_SetSelectable(
                CSF_AT(void*, self, 0x148), nullptr,
                static_cast<char>(endPosition < limit)));
        }

        CSF_AT(char, self, 0x16A) = 0;
    }

    void* control = CSF_AT(void*, self, 0x144);
    if (control != nullptr)
    {
        result = static_cast<char>(
            CGuiSlider_IsSelectableAndNotType10(control, nullptr));
        if (result != 0)
        {
            control = CSF_AT(void*, self, 0x144);
            result = static_cast<char>(
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, control, 0)[0x134 / 4])(control, nullptr));
        }
    }

    control = CSF_AT(void*, self, 0x148);
    if (control != nullptr)
    {
        result = static_cast<char>(
            CGuiSlider_IsSelectableAndNotType10(control, nullptr));
        if (result != 0)
        {
            control = CSF_AT(void*, self, 0x148);
            result = static_cast<char>(
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, control, 0)[0x134 / 4])(control, nullptr));
        }
    }

    control = CSF_AT(void*, self, 0x14C);
    if (control != nullptr)
    {
        result = static_cast<char>(
            CGuiSlider_IsSelectableAndNotType10(control, nullptr));
        if (result != 0)
        {
            control = CSF_AT(void*, self, 0x14C);
            result = static_cast<char>(
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, control, 0)[0x134 / 4])(control, nullptr));
        }
    }

    return result;
}

HOOK(0x0072CFD0, RefreshChildControlStates, "auto");
