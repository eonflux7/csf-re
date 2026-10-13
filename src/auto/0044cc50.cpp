#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a C_008311c4 object to its default state and returns the object.
static C_008311c4* __fastcall C_008311c4_InitDefaults(C_008311c4* self, void* /*edx*/) {
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D428);
    self->vtbl = CSF_VTBL(0x008311C4);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x008311A8);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0xFFFFFFFFu;
    self->field_0020 = 0;
    self->field_0008 = 0;
    return self;
}

HOOK(0x0044CC50, C_008311c4_InitDefaults, "auto");
