#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00725540 (16 bytes): stores a2 into CGuiButton::field_01e8 (+0x1E8) and returns it.
static int __fastcall CGuiButton_SetField01E8(CGuiButton* self, void* /*edx*/, int value)
{
    self->field_01e8 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x725540, CGuiButton_SetField01E8, "auto");
