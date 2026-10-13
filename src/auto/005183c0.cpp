#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0083f630 constructor: installs C_0083f630's vtable and sets the field at +0x04 to -1.
// __thiscall (ecx = this), ret 0, returns this in eax.
static C_0083f630* __fastcall C_0083f630_Construct(C_0083f630* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x0083F630);
    self->field_0004 = 0xFFFFFFFFu;
    return self;
}

HOOK(0x005183C0, C_0083f630_Construct, "auto");

