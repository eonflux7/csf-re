#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int (__fastcall* const CGuiControl_IsFocusEligible)(CGuiControl*, void*) =
    reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(0x00718700);

static int (__fastcall* const CGuiControl_ResolveFocusState)(CGuiControl*, void*) =
    reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(0x00718660);

static int __fastcall CGuiPanel_SetFocusedControl(
    CGuiPanel* self, void* /*edx*/, CGuiControl* control)
{
    int result =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x244 / 4])(self, nullptr);

    if (static_cast<uint8_t>(result) != 0)
        return result;

    auto* previous = reinterpret_cast<CGuiControl*>(self->field_014c);
    if (previous != nullptr)
    {
        result =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, previous, 0)[0xB8 / 4])(
                    previous, nullptr, 0);
        self->field_014c = 0;
    }

    if (control == nullptr)
        return result;

    const uint8_t acceptsFocus =
        reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr);

    if (acceptsFocus != 0 &&
        static_cast<uint8_t>(
            CGuiControl_IsFocusEligible(control, nullptr)) != 0)
    {
        self->field_014c = reinterpret_cast<uint32_t>(control);
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xB8 / 4])(control, nullptr, 1);
    }
    else
    {
        self->field_014c =
            reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x15C / 4])(self, nullptr);
    }

    return CGuiControl_ResolveFocusState(self, nullptr);
}

HOOK(0x0072A4F0, CGuiPanel_SetFocusedControl, "auto");
