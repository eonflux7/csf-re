#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor for C_008343a4 (HUD alert icon). Sets the vtable, zeroes the
// state fields, and sets the two icon-size fields (+0x1C, +0x2C) to 10.
static C_008343a4* __fastcall C_008343a4_Construct(C_008343a4* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x008343A4);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 10;
    self->field_0004 = 0;
    self->field_0030 = 0;
    return self;
}

HOOK(0x0049FF90, C_008343a4_Construct, "auto");
