#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// CGuiChoiceNumButton::SetValue(int value)
// If `value` lies within the inclusive range [field_01f0, field_01f8], store it
// into field_0204 and notify the owning object through virtual slot 0x2B0
// (index 172) with argument 0, returning that call's result.
// Otherwise returns the incoming value untouched.
static int __fastcall CGuiChoiceNumButton_SetValue(CGuiChoiceNumButton* self, void* /*edx*/, int value)
{
    if (value >= static_cast<int>(self->field_01f0) && value <= static_cast<int>(self->field_01f8))
    {
        self->field_0204 = static_cast<uint32_t>(value);
        void** vtbl = *reinterpret_cast<void***>(self);
        return ((int(__fastcall*)(void*, void*, int))vtbl[0x2b0 / 4])(self, nullptr, 0);
    }
    return value;
}

HOOK(0x72F850, CGuiChoiceNumButton_SetValue, "auto");
