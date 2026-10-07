#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00725560 (16 bytes): stores a2 into CGuiButton::field_01E0 (+0x1E0) and returns it.
static int __fastcall CGuiButton_SetField01E0(CGuiButton* self, void* /*edx*/, int value)
{
    self->field_01e0 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x725560, CGuiButton_SetField01E0, "auto");
