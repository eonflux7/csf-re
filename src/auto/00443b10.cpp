#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor for C_0083109c (base C_0082d410). Writes the base-class and
// class vtables, zeroes the state fields, sets two fields to 10, and returns this.
static C_0083109c* __fastcall C_0083109c_Construct(C_0083109c* self, void* /*edx*/) {
    // Base-class stage: field_0004 is first set to the address of data 0x82D418.
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D418);
    self->vtbl = CSF_VTBL(0x0083109C);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x00831094);

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 10;
    self->field_0008 = 0;

    return self;
}

HOOK(0x00443B10, C_0083109c_Construct, "auto");
