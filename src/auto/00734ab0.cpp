#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_SetValueBits(C_008670f0* self, void* /*edx*/, int value)
{
    std::memcpy(&self->field_0100, &value, sizeof(value));
    return value;
}

HOOK(0x734AB0, C_008670f0_SetValueBits, "auto");
