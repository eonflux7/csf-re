#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0085ec34 is a 12-byte object. This is its constructor: it installs the
// class vtable, clears the two value fields at +0x4 and +0x8, and returns
// `this` (eax holds the object's address on return).
static uint32_t* __fastcall C_0085ec34_Construct(C_0085ec34* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0085EC34);
    self->field_0004 = 0;
    self->field_0008 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6A0260, C_0085ec34_Construct, "auto");
