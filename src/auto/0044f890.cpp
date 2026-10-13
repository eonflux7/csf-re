#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetEditIntegerProperty(CGuiControl* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x1A4) = value;
    return value;
}

HOOK(0x44F890, CGuiControl_SetEditIntegerProperty, "auto");
