#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer (C_00830d68 chain), __fastcall with (this, edx), returns void in practice.
CSF_FUNC(int, __fastcall, C_00830d68_Init, 0x0043ABA0, (void* self, void* edx));

// Constructor of C_00830d90 (minimap radar object): runs the base initializer,
// installs this class's vtable and zeroes the float at +0x48. Returns this.
static C_00830d90* __fastcall C_00830d90_Construct(C_00830d90* self, void* /*edx*/) {
    C_00830d68_Init(self, nullptr);
    self->vtbl = CSF_VTBL(0x00830D90);
    self->field_0048 = 0.0f;
    return self;
}

HOOK(0x0043AD40, C_00830d90_Construct, "auto");
