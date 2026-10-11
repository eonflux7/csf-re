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

// Callee: unnamed vehicle handler, __thiscall-equivalent (self in ecx, a2 on stack, ret 4).
CSF_FUNC(char, __fastcall, sub_41AEC0, 0x0041AEC0, (void* self, void* edx, int a2));

// Original: 0x0041B5D0, __thiscall, ret 4.
// Returns the flag byte when clear; otherwise tail-forwards the call to sub_41AEC0.
static char __fastcall C_0082e280_ForwardIfFlagSet(C_0082e280* self, void* /*edx*/, int a2)
{
    char result = self->field_000c;
    if (result)
        return sub_41AEC0(self, nullptr, a2);
    return result;
}

HOOK(0x0041B5D0, C_0082e280_ForwardIfFlagSet, "auto");
