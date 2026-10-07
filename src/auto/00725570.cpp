#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00725570 (16 bytes): stores a2 into CGuiButton::field_01E4 and returns it.
// The same shape as CGuiControl's input-state setter; here it is the button's builder scalar at +0x1E4.
static int __fastcall CGuiButton_SetField01E4(CGuiButton* self, void* /*edx*/, int value)
{
    self->field_01e4 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x725570, CGuiButton_SetField01E4, "auto");
