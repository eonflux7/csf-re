#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class constructor (C_00860178), __thiscall, no stack args.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

// Initializes a bone record: runs the base constructor, stores six
// stack arguments (in push order a2..a7), installs the C_008633a0 vtable,
// zeroes field_001c, sets field_0020 to 1, and returns this (in eax).
static int __fastcall C_008633a0_InitializeBone(C_008633a0* self, void* /*edx*/,
                                                int argA2, int argA3,
                                                int argA4, int argA5,
                                                int argA6, int argA7) {
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->field_0004 = static_cast<uint32_t>(argA2);
    self->field_000c = static_cast<uint32_t>(argA3);
    self->field_0010 = static_cast<uint32_t>(argA4);
    self->field_0014 = static_cast<uint32_t>(argA5);
    self->vtbl = CSF_VTBL(0x008633A0);
    self->field_0018 = static_cast<uint32_t>(argA6);
    self->field_001c = 0;
    self->field_0020 = 1;
    self->field_0008 = static_cast<uint32_t>(argA7);
    return reinterpret_cast<int>(self);
}

HOOK(0x007075F0, C_008633a0_InitializeBone, "auto");
