#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets the S_00471340 state block to its default values and returns a pointer to its first byte.
static uint8_t* __fastcall S_00471340_InitDefaults(S_00471340* self, void* /*edx*/) {
    self->field_0000 = 0;
    self->field_0028 = 1;
    self->field_0029 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 10;
    self->field_0040 = 0;
    self->field_0041 = 0;
    self->field_003c = 0;
    return &self->field_0000;
}

HOOK(0x69B220, S_00471340_InitDefaults, "auto");
