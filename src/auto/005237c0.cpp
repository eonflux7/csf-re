#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_00840ff0_Construct(C_00840ff0* self, void* /*edx*/) {
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    CSF_AT(void*, self, 0x28) = CSF_VTBL(0x0082D448);
    self->field_002c = 0xFFFFFFFFu;
    self->field_0030 = 0xFFFFFFFFu;
    self->field_000c = 0xFFFFFFFFu;
    self->field_0010 = 0xFFFFFFFFu;
    self->field_0004 = 1;
    self->field_0008 = 1;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0038 = 0;
    self->field_0039 = 1;
    self->field_003a = 1;
    self->field_0034 = 0.0f;
    self->field_003c = 0;
    self->vtbl = CSF_VTBL(0x00840FF0);
    self->field_0058 = 15;
    self->field_0054 = 0;
    CSF_AT(uint8_t, self, 0x44) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x005237C0, C_00840ff0_Construct, "auto");
