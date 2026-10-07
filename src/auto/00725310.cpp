#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base setter at 0x718C00 (__thiscall(int* this, int a2), ret 4) — tail-jumped to by the original.
CSF_FUNC(int, __fastcall, CGuiControl_SetValue_718C00, 0x718C00, (int* self, void* edx, int a2));

// Stores the given value into the button (field_01d0) and into its fx object (fx+0x1C),
// then forwards the whole call to the base setter.
static int __fastcall CGuiButton_SetFxValue(CGuiButton* self, void* /*edx*/, int a2)
{
    uint32_t pFx = self->m_pFx;
    *reinterpret_cast<int*>(&self->field_01d0) = a2;
    *reinterpret_cast<int*>(pFx + 0x1c) = a2;
    return CGuiControl_SetValue_718C00(reinterpret_cast<int*>(self), nullptr, a2);
}

HOOK(0x725310, CGuiButton_SetFxValue, "auto");
