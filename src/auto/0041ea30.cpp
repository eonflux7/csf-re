#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor for C_0082e6a0 (base C_0082e664 : C_0082d410).
// Zeroes the state fields, sets two counters to 10, installs this class's vtable,
// and returns the object pointer (the original returns &this->__vftable == this).
static uint32_t* __fastcall C_0082e6a0_Construct(C_0082e6a0* self, void* /*edx*/) {
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;
    self->field_0014 = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    self->field_001c = 0;
    self->field_0020 = 10;
    self->vtbl = CSF_VTBL(0x0082E6A0);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0041EA30, C_0082e6a0_Construct, "auto");
