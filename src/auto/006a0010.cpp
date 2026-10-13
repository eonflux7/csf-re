#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0085ec18 constructor: installs C_0085ec18's vtable and zeroes the field at +0x04.
// __thiscall (ecx = this), ret 0, returns this in eax.
static C_0085ec18* __fastcall C_0085ec18_Construct(C_0085ec18* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x0085EC18);
    self->field_0004 = 0;
    return self;
}

HOOK(0x006A0010, C_0085ec18_Construct, "auto");
