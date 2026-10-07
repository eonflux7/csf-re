#include "types/auto_classes.h"
#include "hook.h"

// Sets the float value at offset 0x10 to 0 (cleared).
static void __fastcall C_00861bd8_ClearField0010(C_00861bd8* self, void* /*edx*/)
{
    self->field_0010 = 0.0f;
}

HOOK(0x006E5090, C_00861bd8_ClearField0010, "auto");
