#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00751F10: stores the byte argument into the object's field at +0x1C and returns it.
static char __fastcall C_00876780_SetField1c(C_00876780* self, void* /*edx*/, char value)
{
    self->field_001c = value;
    return value;
}

HOOK(0x751F10, C_00876780_SetField1c, "auto");
