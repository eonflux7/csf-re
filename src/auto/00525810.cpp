#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_00841090_Initialize(C_00841090* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00841090);
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0028 = -1;
    self->field_002c = -1;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 10;
    *reinterpret_cast<uint8_t*>(&self->field_0004) = 0;
    self->field_000c = 0;
    self->field_0008 = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = -1;
    self->field_0020 = -1;
    self->field_0024 = -1;
    self->field_0060 = 2130706431;
    self->field_0064 = 2130706431;
    self->field_0068 = 2130706431;
    self->field_006c = 2130706431;
    self->field_0070 = 2130706431;
    self->field_0074 = 2130706431;
    self->field_0078 = 2130706431;
    self->field_007c = 2130706431;
    self->field_0080 = 2130706431;
    self->field_0084 = 2130706431;
    self->field_0088 = 2130706431;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00525810, C_00841090_Initialize, "auto");
