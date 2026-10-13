#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets every member of a C_00860908 to its default state and returns the object.
static C_00860908* __fastcall C_00860908_InitDefaults(C_00860908* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00860908);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0x3f800000u;  // float 1.0 bit pattern
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->field_002c = 0;
    self->field_002d = 0;
    return self;
}

HOOK(0x006C8670, C_00860908_InitDefaults, "auto");
