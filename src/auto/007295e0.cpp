#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x007295E0: stores the byte argument into this+0x40 and returns it unchanged.
static char __fastcall C_00865a10_SetField0040(C_00865a10* self, void* /*edx*/, char value)
{
    self->field_0040 = value;
    return value;
}

HOOK(0x007295E0, C_00865a10_SetField0040, "auto");
