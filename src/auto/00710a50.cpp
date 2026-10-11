#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a C_00863ba8 audio-effect object to its default state.
// Returns the object pointer (eax = this in the original).
static C_00863ba8* __fastcall C_00863ba8_ResetDefaults(C_00863ba8* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x00863BA8);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0032 = 1;
    self->field_0014 = 0;
    self->field_0018 = 1;
    self->field_001c = 1;
    self->field_0020 = 0;
    self->field_0028 = 0;
    self->field_0030 = 1;
    self->field_0031 = 0;
    self->field_0024 = 0;
    self->field_002c = 1.0f;

    return self;
}

HOOK(0x00710A50, C_00863ba8_ResetDefaults, "auto");
