#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor of C_00860178, bound by address.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

// Constructs a C_0086315c emitter-parameter block: runs the base constructor,
// installs this class's vtable, and zeroes the parameter fields.
static int __fastcall C_0086315c_ConstructEmitterParams(C_0086315c* self, void* /*edx*/) {
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0086315C);
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x00701050, C_0086315c_ConstructEmitterParams, "auto");
