#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets the default state of C_0082d448 (char_action_fsm). Stores the class vtable
// at +0x00 and +0x10, sets sentinel (-1) ids and clears flags/counters. Returns this.
static C_0082d448* __fastcall C_0082d448_InitDefaults(C_0082d448* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0082D448);
    CSF_AT(void*, self, 0x10) = CSF_VTBL(0x0082D448);

    self->field_0014 = 0xFFFFFFFFu;
    self->field_0018 = 0xFFFFFFFFu;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_000c = 0;
    self->field_0028 = 0xFFFFFFFFu;

    self->field_002c = 1;
    self->field_002d = 0;
    self->field_002e = 0;
    self->field_002f = 0;
    self->field_0030 = 1;
    self->field_0031 = 0;

    self->field_0004 = 0xFFFFFFFFu;
    self->field_0008 = 0xFFFFFFFFu;

    return self;
}

HOOK(0x4810B0, C_0082d448_InitDefaults, "auto");
