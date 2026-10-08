#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_OnKeyDown, 0x007184C0,
         (CGuiControl* self, void* edx, int key, int context));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiButton_ActivateLinkedObject, 0x00725740,
         (CGuiButton* self, void* edx));

static char __fastcall CGuiButton_OnKeyDown(
    CGuiButton* self, void* /*edx*/, int key, int context)
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

    if (key == 28 || key == 156)
    {
        void* link = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0010), 0);
        void* associatedObject = CSF_AT(void*, link, 0);
        if (associatedObject)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, associatedObject, 0)[0x24 / 4])(
                    associatedObject, nullptr, 0);
        }
        CGuiButton_ActivateLinkedObject(self, nullptr);
    }

    return CGuiControl_OnKeyDown(self, nullptr, key, context);
}

HOOK(0x00726020, CGuiButton_OnKeyDown, "auto");
