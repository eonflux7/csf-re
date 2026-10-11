#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor-style initializer: stores a data pointer at +0x4, overwrites it
// with the C_0082d410 vtable address at +0x4, then sets the primary vtable at +0x0.
static void __fastcall C_0082d410_SetVtablesAndDataPointer(C_0082d410* self, void* /*edx*/) {
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083E144);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0082D410);
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0082D410);
}

HOOK(0x004E2C60, C_0082d410_SetVtablesAndDataPointer, "auto");
