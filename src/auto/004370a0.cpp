#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetStoredValue(CGuiControl* self, void* /*edx*/, int value)
{
    self->field_011c = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x4370A0, CGuiControl_SetStoredValue, "auto");
