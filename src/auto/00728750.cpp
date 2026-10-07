#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00728750: stores the byte argument at this+0x34 and returns it.
static char __fastcall C_008658f8_SetValueAt34(C_008658f8* self, void* /*edx*/, char value)
{
    self->field_0034 = value;
    return value;
}

HOOK(0x00728750, C_008658f8_SetValueAt34, "auto");
