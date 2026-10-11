#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Initializes a C_0082d410 object: installs a temporary data table pointer, clears
// field_0008, then restores the class vtable in both the vptr slot and field_0004.
static void __fastcall C_0082d410_Init(C_0082d410* self, void* /*edx*/) {
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D690);
    self->field_0008 = 0;
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    self->vtbl = CSF_VTBL(0x0082D410);
}

HOOK(0x004063D0, C_0082d410_Init, "auto");
