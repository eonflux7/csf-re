// Reimplementation of the CGuiSwitch setter at 0x00730280.
// Stores a 32-bit value at CGuiSwitch+0x17C and tail-forwards to the base handler 0x718C00.

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Tail-called base handler: __thiscall(int* this, int a2), ret 4.
CSF_FUNC(int, __fastcall, CGuiControl_ForwardEventToFirstEffect, 0x718C00, (int* self, void* /*edx*/, int a2));

static int __fastcall CGuiSwitch_SetField017c(CGuiSwitch* self, void* /*edx*/, int value)
{
    // Original stores the raw 32-bit argument (mov [ecx+0x17C], eax): no conversion.
    *reinterpret_cast<int*>(&self->field_017c) = value;
    return CGuiControl_ForwardEventToFirstEffect(reinterpret_cast<int*>(self), nullptr, value);
}

HOOK(0x730280, CGuiSwitch_SetField017c, "auto");
