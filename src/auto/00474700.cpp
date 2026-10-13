#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor-style initializer for C_00831618: sets both sentinel
// slots to -1, points the two embedded base-table fields at the
// C_0082d448 table, stores the constructor argument, zeroes the
// remaining state, and installs the C_00831618 vtable.
static uint32_t* __fastcall C_00831618_InitializeImpactRecord(C_00831618* self, void* /*edx*/, int a2) {
    self->field_000c = static_cast<uint32_t>(-1);
    self->field_0010 = static_cast<uint32_t>(-1);
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D448));
    self->field_0018 = static_cast<uint32_t>(-1);
    self->field_001c = static_cast<uint32_t>(-1);
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D448));
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->vtbl = CSF_VTBL(0x00831618);
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00474700, C_00831618_InitializeImpactRecord, "auto");
