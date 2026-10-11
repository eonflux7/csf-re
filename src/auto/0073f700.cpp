#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// DNameNode constructor (C_00867768): installs C_00867768's vtable and zeroes the field at +0x04.
// __thiscall (ecx = this), ret 0, returns this in eax.
static C_00867768* __fastcall C_00867768_Construct(C_00867768* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x00867768);
    self->field_0004 = 0;
    return self;
}

HOOK(0x0073F700, C_00867768_Construct, "auto");
