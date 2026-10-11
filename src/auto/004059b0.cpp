#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Initializer: writes two pointers to +0x4 (the second overwrites the first),
// then sets the primary vtable at +0x0.
static void __fastcall C_0082d410_InitVtables(C_0082d410* self, void* /*edx*/) {
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D570);
    self->field_0004 = (uint32_t)CSF_VTBL(0x0082D410);
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0082D410);
}

HOOK(0x004059B0, C_0082d410_InitVtables, "auto");
