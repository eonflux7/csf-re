#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// __thiscall method: stores the argument at offset +0x28 and returns it.
static int __fastcall C_008630e0_SetField0028(C_008630e0* self, void* /*edx*/, int value)
{
    self->field_0028 = value;
    return value;
}

HOOK(0x0047E100, C_008630e0_SetField0028, "auto");
