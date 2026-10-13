#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00871914 initializer: installs the C_00871914 vtable, zeroes the three
// data fields at +0x08, +0x0C and +0x10, and returns the object address
// (pointer to the vtable slot).
static uint32_t* __fastcall C_00871914_Init(C_00871914* self, void* /*edx*/) {
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00871914);
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x007711C0, C_00871914_Init, "auto");
