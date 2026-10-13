#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor-style initializer for the scene light/object parser node (C_0083ef30).
// Stores the base-class and own vtables, zeroes the state fields, sets the
// default counters to 10, records the owner passed in a2, and returns this.
static uint32_t* __fastcall C_0083ef30_InitDefaults(C_0083ef30* self, void* /*edx*/, int a2)
{
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082D428)));
    self->vtbl = CSF_VTBL(0x0083EF30);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0083EF10)));

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_0028 = 10;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0038 = 10;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0008 = static_cast<uint32_t>(a2);
    self->field_0040 = 1;
    self->field_003c = 0.0f;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004F0E10, C_0083ef30_InitDefaults, "auto");
