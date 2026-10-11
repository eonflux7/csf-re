#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_008310f8_Initialize(
    C_008310f8* self, void* /*edx*/, int owner)
{
    self->field_000c = -1;
    self->field_0010 = -1;
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0018 = -1;
    self->field_001c = -1;
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0004 = owner;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->vtbl = CSF_VTBL(0x008310F8);
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00446550, C_008310f8_Initialize, "auto");
