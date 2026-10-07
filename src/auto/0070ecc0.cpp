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

// Returns field_0034 minus 6.
static int __fastcall C_00863ac0_GetValueMinus6(C_00863ac0* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0034) - 6;
}

HOOK(0x0070ECC0, C_00863ac0_GetValueMinus6, "auto");
