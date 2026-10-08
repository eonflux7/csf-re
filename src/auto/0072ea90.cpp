#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (CGuiChoiceButton* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_SetField0148, 0x00725120,
         (CGuiChoiceButton* self, void* edx, int value));
CSF_FUNC(void, __fastcall, CGuiButton_StartTextColorEffect, 0x00725740,
         (CGuiChoiceButton* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiButton_HandleEvent, 0x00725DA0,
         (CGuiChoiceButton* self, void* edx, int event));
CSF_FUNC(int, __fastcall, CGuiSlider_GetField0148, 0x00730110,
         (CGuiChoiceButton* self, void* edx));

static int __fastcall CGuiChoiceButton_HandleEvent(
    CGuiChoiceButton* self, void* /*edx*/, int event)
{
    if (event == 19)
    {
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[80])(self, nullptr) & 1) != 0 &&
            CGuiSlider_GetField0148(self, nullptr) == 2)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, self, 0)[175])(self, nullptr, 1);
        }
    }
    else if (event == 21)
    {
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[80])(self, nullptr) & 1) != 0)
        {
            CGuiSlider_ForwardSelfToParent(self, nullptr);
            CGuiControl_SetField0148(self, nullptr, 2);
        }
    }
    else if (event == 22)
    {
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[80])(self, nullptr) & 1) != 0 &&
            CGuiSlider_GetField0148(self, nullptr) == 2)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, self, 0)[175])(self, nullptr, 0);

            void* parent = **reinterpret_cast<void***>(self->field_0010);
            if (parent)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, parent, 0)[9])(parent, nullptr, 0);
            }

            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, self, 0)[78])(self, nullptr, 13);
            CGuiButton_StartTextColorEffect(self, nullptr);
        }
    }

    return CGuiButton_HandleEvent(self, nullptr, event);
}

HOOK(0x0072EA90, CGuiChoiceButton_HandleEvent, "auto");
