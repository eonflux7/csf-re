#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Subtracts a float amount from the field at +0x3C of C_0083ef30.
// Original: fld [ecx+0x3C]; fsub [ebp+8]; fstp [ecx+0x3C]; ret 4
static void __fastcall C_0083ef30_SubtractAmountFromField003C(C_0083ef30* self, void* /*edx*/, float amount) {
    self->field_003c = self->field_003c - amount;
}

HOOK(0x004F0CB0, C_0083ef30_SubtractAmountFromField003C, "auto");
