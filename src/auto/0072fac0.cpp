#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (CGuiChoiceNumButton* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_SetField0148, 0x00725120,
         (CGuiChoiceNumButton* self, void* edx, int state));
CSF_FUNC(void, __fastcall, CGuiControl_FinalizeInteraction, 0x00725740,
         (CGuiChoiceNumButton* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiButton_HandleEvent, 0x00725DA0,
         (CGuiChoiceNumButton* self, void* edx, int event));
CSF_FUNC(int, __fastcall, CGuiSlider_GetField0148, 0x00730110,
         (CGuiChoiceNumButton* self, void* edx));

static int __fastcall CGuiChoiceNumButton_HandleNumericChoiceEvent(
    CGuiChoiceNumButton* self, void* /*edx*/, int event)
{
    if (event == 19)
    {
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) & 1) != 0
            && CGuiSlider_GetField0148(self, nullptr) == 2)
        {
            const std::int32_t adjustment =
                static_cast<std::int32_t>(self->field_0200);
            ((void(__fastcall*)(void*, void*, std::int32_t))
                 CSF_AT(void**, self, 0)[0x2B0 / 4])(
                     self, nullptr, adjustment);
        }
    }
    else if (event == 21)
    {
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) & 1) != 0)
        {
            CGuiSlider_ForwardSelfToParent(self, nullptr);
            CGuiControl_SetField0148(self, nullptr, 2);
        }
    }
    else if (event == 22)
    {
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) & 1) != 0
            && CGuiSlider_GetField0148(self, nullptr) == 2)
        {
            const std::int32_t adjustment =
                static_cast<std::int32_t>(0u - self->field_0200);
            ((void(__fastcall*)(void*, void*, std::int32_t))
                 CSF_AT(void**, self, 0)[0x2B0 / 4])(
                     self, nullptr, adjustment);

            void* link = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0010), 0);
            void* relatedObject = CSF_AT(void*, link, 0);
            if (relatedObject)
            {
                ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, relatedObject, 0)[0x24 / 4])(
                         relatedObject, nullptr, 0);
            }

            ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
            CGuiControl_FinalizeInteraction(self, nullptr);
        }
    }

    return CGuiButton_HandleEvent(self, nullptr, event);
}

HOOK(0x0072FAC0, CGuiChoiceNumButton_HandleNumericChoiceEvent, "auto");
