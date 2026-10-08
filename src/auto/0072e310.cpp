#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_HandleSelectionInput_718510, 0x00718510,
         (CGuiControl* self, void* edx, int input, int inputData));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiSlider* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiSlider_RedirectConsoleArrowSelection, 0x0072D760,
         (CGuiSlider* self, void* edx));

static char __fastcall CGuiSlider_HandleSliderInput(
    CGuiSlider* self, void* /*edx*/, int input, int inputData)
{
    char result =
        ((char(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x6C / 4])(
            self, nullptr);
    if (!result)
        return result;

    result =
        ((char(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xBC / 4])(
            self, nullptr);
    if (!result)
        return result;

    result = static_cast<char>(
        CGuiSlider_IsSelectableAndNotType10(self, nullptr));
    if (!result)
        return result;

    if (input == 5)
    {
        void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
        if (arrowUp &&
            ((char(__fastcall*)(void*, void*))CSF_AT(void**, arrowUp, 0)[0xBC / 4])(
                arrowUp, nullptr))
        {
            const uint32_t position = self->m_pos - 1u;
            ((void(__fastcall*)(void*, void*, uint32_t, int))
                CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, position, 1);
        }

        void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
        if (arrowDown &&
            ((char(__fastcall*)(void*, void*))CSF_AT(void**, arrowDown, 0)[0xBC / 4])(
                arrowDown, nullptr))
        {
            const uint32_t position = self->m_pos + 1u;
            ((void(__fastcall*)(void*, void*, uint32_t, int))
                CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, position, 1);
        }
    }
    else
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, self, 0)[0x288 / 4])(self, nullptr, input);
    }

    void* thumb = reinterpret_cast<void*>(self->m_pThumb);
    if (thumb &&
        ((char(__fastcall*)(void*, void*))CSF_AT(void**, thumb, 0)[0xBC / 4])(
            thumb, nullptr))
    {
        const uint32_t orientation = self->m_bHorizontal;
        const uint32_t oldPosition = self->m_pos;

        if ((orientation == 0 && input == 0) ||
            (orientation == 1 && input == 2))
        {
            ((void(__fastcall*)(void*, void*, uint32_t, int))
                CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, oldPosition - 1u, 0);
        }
        else if ((orientation == 0 && input == 1) ||
                 (orientation == 1 && input == 3))
        {
            ((void(__fastcall*)(void*, void*, uint32_t, int))
                CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, oldPosition + 1u, 0);
        }

        if (oldPosition != self->m_pos)
        {
            void* context = reinterpret_cast<void*>(self->field_0010);
            void* contextData = CSF_AT(void*, context, 0);
            void* eventTarget = CSF_AT(void*, contextData, 4);
            if (eventTarget)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, eventTarget, 0)[0x24 / 4])(
                        eventTarget, nullptr, 0);
            }

            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
        }
    }

    void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
    if (arrowUp)
    {
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, arrowUp, 0)[0x38 / 4])(
                arrowUp, nullptr, input, inputData);
    }

    void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
    if (arrowDown)
    {
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, arrowDown, 0)[0x38 / 4])(
                arrowDown, nullptr, input, inputData);
    }

    thumb = reinterpret_cast<void*>(self->m_pThumb);
    if (thumb)
    {
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, thumb, 0)[0x38 / 4])(
                thumb, nullptr, input, inputData);
    }

    CGuiControl_HandleSelectionInput_718510(self, nullptr, input, inputData);
    return CGuiSlider_RedirectConsoleArrowSelection(self, nullptr);
}

HOOK(0x0072E310, CGuiSlider_HandleSliderInput, "auto");
