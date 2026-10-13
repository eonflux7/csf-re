#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor for C_0083ef04 (scene mesh object). Installs the class vtable,
// zeroes the first three state fields, sets field_0010 to 10, and returns
// the object pointer (the original returns &this->__vftable, which is `this`).
static uint32_t* __fastcall C_0083ef04_Construct(C_0083ef04* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0083EF04);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004F0990, C_0083ef04_Construct, "auto");
