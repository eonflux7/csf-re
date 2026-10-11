#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0085ec74 constructor: installs C_0085ec74's vtable and zeroes the field at +0x04.
// __thiscall (ecx = this), ret 0, returns this in eax.
static C_0085ec74* __fastcall C_0085ec74_Construct(C_0085ec74* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0085EC74);
    self->field_0004 = 0;
    return self;
}

HOOK(0x006A02B0, C_0085ec74_Construct, "auto");
