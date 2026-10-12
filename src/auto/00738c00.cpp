#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores newValue into the object's field at +0x08 only when it differs from
// the current value, and returns newValue. __thiscall with one stack argument, ret 4.
static int __fastcall C_008673b0_SetField0008IfChanged(C_008673b0* self, void* /*edx*/, int newValue) {
    if (newValue != self->field_0008) {
        self->field_0008 = newValue;
    }
    return newValue;
}

HOOK(0x00738C00, C_008673b0_SetField0008IfChanged, "auto");
