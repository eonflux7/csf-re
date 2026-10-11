#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00840af8 constructor: installs the C_00840af8 vtable, zeroes the three
// data fields at +0x08, +0x0C and +0x10, and returns the object address.
static uint32_t* __fastcall C_00840af8_Construct(C_00840af8* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00840AF8);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00516D00, C_00840af8_Construct, "auto");
