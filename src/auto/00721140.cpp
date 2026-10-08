#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280,
         (void* self, void* edx, int eventId));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (void* self, void* edx));

static int __fastcall CGuiEdit_SendEvent(
    CGuiEdit* self, void* /*edx*/, int eventId)
{
    void* actionTarget;

    switch (eventId)
    {
    case 13:
    {
        void* actions = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0010), 0);
        actionTarget = CSF_AT(void*, actions, 0);
        break;
    }

    case 15:
    {
        void* owner =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x10C / 4])(self, nullptr);

        void* focusedControl =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, owner, 0)[0x104 / 4])(owner, nullptr);

        if (focusedControl &&
            ((bool (__fastcall*)(void*, void*))
                CSF_AT(void**, focusedControl, 0)[0x188 / 4])(
                    focusedControl, nullptr))
        {
            return CGuiControl_SendEvent(self, nullptr, eventId);
        }

        if ((((int (__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(
                      self, nullptr) & 1) == 0)
        {
            return CGuiControl_SendEvent(self, nullptr, eventId);
        }

        CGuiSlider_ForwardSelfToParent(self, nullptr);

        if ((((int (__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(
                      self, nullptr) & 0x10) != 0)
        {
            return CGuiControl_SendEvent(self, nullptr, eventId);
        }

        void* actions = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0010), 0);
        actionTarget = CSF_AT(void*, actions, 4);
        break;
    }

    case 18:
        CGuiSlider_ForwardSelfToParent(self, nullptr);
        ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
        return CGuiControl_SendEvent(self, nullptr, eventId);

    default:
        return CGuiControl_SendEvent(self, nullptr, eventId);
    }

    if (actionTarget)
    {
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, actionTarget, 0)[0x24 / 4])(
                actionTarget, nullptr, 0);
    }

    return CGuiControl_SendEvent(self, nullptr, eventId);
}

HOOK(0x00721140, CGuiEdit_SendEvent, "auto");
