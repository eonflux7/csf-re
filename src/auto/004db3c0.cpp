#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Initializer: stores the address of data block 0x83DE40 into +0x4, then overwrites +0x4
// with the C_0082d410 vtable, then sets the primary vtable at +0x0.
static void __fastcall C_0082d410_InitTaskVtables(C_0082d410* self, void* /*edx*/) {
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083DE40);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0082D410);
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0082D410);
}

HOOK(0x004DB3C0, C_0082d410_InitTaskVtables, "auto");
