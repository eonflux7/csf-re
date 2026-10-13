#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor of C_00830438: installs the class vtable, zeroes the two
// data fields, and returns the object address (the vtable slot).
static uint32_t* __fastcall C_00830438_Construct(C_00830438* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00830438);
    self->field_0004 = 0;
    self->field_0008 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00712CC0, C_00830438_Construct, "auto");
