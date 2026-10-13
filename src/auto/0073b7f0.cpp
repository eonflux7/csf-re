#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets every field of C_0086754c to its default value and installs its vtable.
static uint8_t* __fastcall C_0086754c_ResetDefaults(C_0086754c* self, void* /*edx*/) {
    self->field_0004 = 0;
    self->vtbl = CSF_VTBL(0x86754C);
    self->field_0010 = 0.0f;
    self->field_0014 = 0.0f;
    self->field_0018 = 0;
    self->field_0020 = 0;
    self->field_001c = 0;
    self->field_0024 = 0;
    self->field_0028 = 0.0f;
    self->field_002c = 0.0f;
    self->field_0030 = 0;
    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x0073B7F0, C_0086754c_ResetDefaults, "auto");
