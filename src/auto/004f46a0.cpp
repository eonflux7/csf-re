#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_0083f130_Initialize(C_0083f130* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083F130);
    self->field_001c = 15;
    self->field_0018 = 0;
    reinterpret_cast<uint8_t*>(&self->field_0008)[0] = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0044 = 0;
    self->field_0040 = 0;
    self->field_003c = 0;
    self->field_0038 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0020 = 0;
    self->field_0048 = -1082130432;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004F46A0, C_0083f130_Initialize, "auto");
