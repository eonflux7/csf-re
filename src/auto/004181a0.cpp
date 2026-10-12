#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer at 0x415DA0 (__thiscall, ret 0), called first with the same this.
CSF_FUNC(int, __fastcall, C_0082e128_InitBaseSubobject, 0x00415DA0, (void* self, void* edx));

// Resets the C_0082e128 member state to its default values and returns 0.
static int __fastcall C_0082e128_ResetToDefaultState(C_0082e128* self, void* /*edx*/) {
    C_0082e128_InitBaseSubobject(self, nullptr);

    self->field_0200 = 0;
    CSF_AT(uint32_t, self, 0x204) = 0;
    self->field_01f2 = 0;
    self->field_01f3 = 0;
    self->field_00bc = 0x3e4ccccdu;   // 0.2f
    self->field_01f0 = 0;
    self->field_01f1 = 0;
    self->field_020c = -1.0f;
    self->field_01fc = 0;
    self->field_0210 = 0;
    self->field_01f4 = 0;
    self->field_01f8 = 0.0f;
    self->field_00c0 = 0;
    CSF_AT(uint32_t, self, 0x208) = 0;
    return 0;
}

HOOK(0x004181A0, C_0082e128_ResetToDefaultState, "auto");
