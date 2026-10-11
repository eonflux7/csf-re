#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor body for C_0082d58c: stores the secondary vtable at +0x04, then the
// primary vtable at +0x00, overwrites +0x04 with the final vtable, then zeroes and
// initializes the instance fields. Returns this.
static C_0082d58c* __fastcall C_0082d58c_Construct(C_0082d58c* self, void* /*edx*/) {
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D428);
    CSF_AT(uint32_t, self, 0x0) = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D58C);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D570);

    self->field_0008 = 0;
    self->field_000c = 10.0f;
    self->field_0010 = 0;
    self->field_0011 = 0;
    self->field_0018 = 0.0f;
    self->field_0014 = 0;
    self->field_0020 = 0.0f;
    self->field_001c = 0.0f;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0024 = 0;
    self->field_0025 = 0;

    return self;
}

HOOK(0x00405950, C_0082d58c_Construct, "auto");
