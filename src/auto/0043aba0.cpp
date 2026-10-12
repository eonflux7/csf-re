#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0043A700: unnamed helper that the constructor calls last, with `this` only.
CSF_FUNC(int, __fastcall, MinimapRadar_InitSubState, 0x0043A700, (void* self, void* edx));

// Constructor for C_00830d68 (minimap radar object). Zeroes its fields, sets the
// default values, calls the helper, and returns `this` in eax.
static C_00830d68* __fastcall C_00830d68_Construct(C_00830d68* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00830D68);
    CSF_AT(uint8_t, self, 0x008) = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0004 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_003c = 0;
    self->field_0040 = 0x3F800000u; // 1.0f bit pattern
    self->field_0044 = 0;

    MinimapRadar_InitSubState(self, nullptr);
    return self;
}

HOOK(0x0043ABA0, C_00830d68_Construct, "auto");
