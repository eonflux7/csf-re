#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer at 0x006FF930 (C_008630e0 chain); called first, as in the original.
CSF_FUNC(int, __fastcall, WaterBase_InitBase, 0x006FF930, (void* self, void* edx));

// Constructor-style init for C_00863600: runs the base init, installs the vtable,
// zeroes four 32-bit members, stores the constructor argument at +0x2C, and returns this.
static C_00863600* __fastcall C_00863600_InitWithArg(C_00863600* self, void* /*edx*/, int a2)
{
    WaterBase_InitBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x00863600);
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0030 = 0;
    self->field_002c = static_cast<uint32_t>(a2);
    return self;
}

HOOK(0x70AA70, C_00863600_InitWithArg, "auto");
