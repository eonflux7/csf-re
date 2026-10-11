#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Initializes an alert/icon HUD object: installs its vtable and sets default state.
// Returns the object pointer (the original returns &this->__vftable, which is `this`).
static uint32_t* __fastcall C_00831278_Initialize(C_00831278* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00831278);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    self->field_002c = 15;
    self->field_0028 = 0;
    CSF_AT(uint8_t, self, 0x18) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4A0FC0, C_00831278_Initialize, "auto");
