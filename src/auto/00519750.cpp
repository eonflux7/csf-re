#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_00840d54_Initialize(C_00840d54* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840D54);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 10;
    self->field_0004 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00519750, C_00840d54_Initialize, "auto");
