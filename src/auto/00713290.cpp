#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CInputDirectInput: stores the two stack arguments into fields +0xC98 and +0xC9C
// (thiscall: ecx = this) and returns the first argument.
static int __fastcall CInputDirectInput_StoreTwoFieldsReturnFirst(CInputDirectInput* self, void* /*edx*/, int firstValue, int secondValue) {
    self->field_0c98 = firstValue;
    self->field_0c9c = secondValue;
    return firstValue;
}

HOOK(0x00713290, CInputDirectInput_StoreTwoFieldsReturnFirst, "auto");
