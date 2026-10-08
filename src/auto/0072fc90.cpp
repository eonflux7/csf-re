#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280,
         (void* self, void* edx, int event));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiButton_HandleEvent, 0x00725DA0,
         (CGuiRadioButton* self, void* edx, int event));

static int __fastcall CGuiRadioButton_HandleEvent(
    CGuiRadioButton* self, void* /*edx*/, int event)
{
    switch (event)
    {
    case 18:
    case 20:
    {
        const int flags =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr);

        if ((flags & 1) != 0)
        {
            auto* linkedObject = CSF_AT(
                C_00830438*, reinterpret_cast<void*>(self->field_0010), 0);
            void* recipient =
                reinterpret_cast<void*>(linkedObject->field_0004);

            if (recipient)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, recipient, 0)[0x24 / 4])(
                        recipient, nullptr, 0);
            }

            CGuiSlider_ForwardSelfToParent(self, nullptr);
        }

        return CGuiControl_SendEvent(self, nullptr, event);
    }

    case 19:
        return CGuiControl_SendEvent(self, nullptr, event);

    default:
        return CGuiButton_HandleEvent(self, nullptr, event);
    }
}

HOOK(0x0072FC90, CGuiRadioButton_HandleEvent, "auto");
