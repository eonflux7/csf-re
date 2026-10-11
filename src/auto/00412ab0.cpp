#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor-style initializer for C_0082d448: installs the vtable and resets the
// animation buffer fields. Returns this (the original returns &this->__vftable, which is this).
static uint32_t* __fastcall C_0082d448_InitColorAnimBuffer(C_0082d448* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0082D448);
    self->field_0004 = 0xFFFFFFFFu;
    self->field_0008 = 0xFFFFFFFFu;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_000c = 0xFFFFFFFFu;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00412AB0, C_0082d448_InitColorAnimBuffer, "auto");
