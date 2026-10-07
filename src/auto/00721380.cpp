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

// 0x00721380 (7 bytes): x87 getter.
//   fld dword ptr [ecx + 0x140]
//   ret
// Loads the 32-bit float field at 0x140 and leaves it in ST(0).
// The returned value is promoted to double by the declared return type;
// on x87 this promotion is exact and emits no extra instruction.
static double __fastcall C_00864e18_GetField0140(C_00864e18* self, void* /*edx*/)
{
    return CSF_AT(float, self, 0x140);
}

HOOK(0x00721380, C_00864e18_GetField0140, "auto");
