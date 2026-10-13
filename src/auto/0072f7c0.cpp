#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiButton_Ctor, 0x00725390,
         (CGuiButton* self, void* edx, int argument1, int argument2));


static CGuiChoiceNumButton* __fastcall CGuiChoiceNumButton_Construct(
    CGuiChoiceNumButton* self, void* /*edx*/, int argument1, int argument2)
{
    CGuiButton_Ctor(self, nullptr, argument1, argument2);

    self->field_0204 = 0;
    self->field_01f4 = 0;
    self->field_01fc = 0;
    CSF_AT(std::uint32_t, self, 0x0) =
        reinterpret_cast<std::uintptr_t>(CSF_VTBL(0x00866600));
    self->m_minValue = 0x80000000u;
    self->field_01f8 = 0x7FFFFFFFu;
    self->field_0200 = 1;

    return self;
}

HOOK(0x0072F7C0, CGuiChoiceNumButton_Construct, "auto");
