#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00725390  int __thiscall CGuiButton_Ctor(int this, int a2, int a3)  ret 8
CSF_FUNC(void, __fastcall, CGuiButton_Ctor, 0x725390, (CGuiButton* self, void* edx, int a2, int a3));

// 0x0072FBB0  CGuiRadioButton constructor: builds the base button then installs the radio vtable.
static CGuiRadioButton* __fastcall CGuiRadioButton_Ctor(CGuiRadioButton* self, void* /*edx*/, int a2, int a3)
{
    CGuiButton_Ctor(self, nullptr, a2, a3);
    CSF_AT(void*, self, 0x0) = reinterpret_cast<void*>(0x8668B8); // CGuiRadioButton vtable
    self->field_01e8 = 0;
    return self;
}

HOOK(0x72FBB0, CGuiRadioButton_Ctor, "auto");
