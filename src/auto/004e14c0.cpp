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

// Callee: C_0085ec18_NullSub (0x004B2A20), __stdcall, 1 stack arg, ret 4
CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));

// Original: void __thiscall sub_4E14C0(C_0083e330* this, int* a2)
// Calls C_0085ec18_NullSub(*a2) when this->field_004C is non-zero. Pops 4 (ret 4).
static void __fastcall C_0083e330_CallNullSubIfFlagSet(C_0083e330* self, void* /*edx*/, int* a2)
{
    if (self->field_004c)
    {
        C_0085ec18_NullSub(*a2);
    }
}

HOOK(0x004E14C0, C_0083e330_CallNullSubIfFlagSet, "auto");
