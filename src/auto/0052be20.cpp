#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, MenuProgressInput_GetGlobalValue, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_SetGlobalInputValue, 0x005231D0,
         (void* self, void* edx, int value));
CSF_FUNC(void, __cdecl, MenuProgressInput_PostInputUpdate, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (void* self, void* edx));

static uint8_t __fastcall CMenuTextInput_SetInputValueAndNotify(
    CMenuTextInput* self, void* /*edx*/, int value)
{
    self->field_001c = static_cast<uint32_t>(value);

    const int globalObject = MenuProgressInput_GetGlobalValue();
    MenuProgressInput_SetGlobalInputValue(
        reinterpret_cast<void*>(globalObject), nullptr, value);
    MenuProgressInput_PostInputUpdate();

    uint8_t result = static_cast<uint8_t>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x40 / 4])(self, nullptr));

    if (!result)
    {
        void* parent = reinterpret_cast<void*>(self->field_0010);
        if (parent)
        {
            self->field_0054 = 0;
            result = static_cast<uint8_t>(
                reinterpret_cast<int(__fastcall*)(void*, void*, CMenuTextInput*, int)>(
                    CSF_AT(void**, parent, 0)[0x60 / 4])(
                        parent, nullptr, self, 0));
        }
    }

    void* inputObject = reinterpret_cast<void*>(self->field_0054);
    if (inputObject)
    {
        result = self->field_0058;
        if (result)
        {
            void* notificationTarget = reinterpret_cast<void*>(self->field_0028);
            if (notificationTarget)
            {
                void* alternateTarget = reinterpret_cast<void*>(self->field_002c);
                result = static_cast<uint8_t>(
                    reinterpret_cast<uintptr_t>(alternateTarget));

                if (alternateTarget)
                {
                    if (!CSF_AT(uint8_t, inputObject, 0x30))
                        notificationTarget = alternateTarget;

                    result = static_cast<uint8_t>(
                        CGuiSlider_ForwardSelfToParent(notificationTarget, nullptr));
                    self->field_0058 = 0;
                }
            }
        }
    }

    return result;
}

HOOK(0x0052BE20, CMenuTextInput_SetInputValueAndNotify, "auto");
