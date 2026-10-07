// Commandos: Strike Force (Pyro Studios) — reimplementation of 0x004E5760
// Clears two 32-bit members of C_0083e9d0 and returns 0.

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

// 0x004E5760: zeroes field_0014 and field_0028, returns 0.
static int __fastcall C_0083e9d0_ResetStateFields(C_0083e9d0* self, void* /*edx*/)
{
    self->field_0014 = 0;
    self->field_0028 = 0;
    return 0;
}

HOOK(0x4E5760, C_0083e9d0_ResetStateFields, "auto");
