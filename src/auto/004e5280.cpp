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

// Subtracts a float amount from the field at +0x48 of C_0083e9d0.
// Original: fld [ecx+0x48]; fsub [ebp+8]; fstp [ecx+0x48]; ret 4
static void __thiscall_placeholder_unused();

static void __fastcall AdjustFieldFloat48ByMinus(C_0083e9d0* self, void* /*edx*/, float amount) {
    self->field_0048 = self->field_0048 - amount;
}

HOOK(0x004E5280, AdjustFieldFloat48ByMinus, "auto");
