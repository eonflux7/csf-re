#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0072F830 (16 bytes): stores a2 into CGuiChoiceNumButton::field_0200 (+0x200) and returns it.
static int __fastcall CGuiChoiceNumButton_SetField0200(CGuiChoiceNumButton* self, void* /*edx*/, int value)
{
    self->field_0200 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x72F830, CGuiChoiceNumButton_SetField0200, "auto");
