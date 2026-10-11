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

// Callee: Autonomous_UpdateBase, __thiscall-equivalent (self in ecx, a2 on stack, ret 4).
CSF_FUNC(char, __fastcall, Autonomous_UpdateBase, 0x004D01C0, (void* self, void* edx, float a2));

// Original: 0x004CEDE0, __thiscall, ret 4.
// Returns the flag byte when clear; otherwise tail-forwards the call to Autonomous_UpdateBase.
static char __fastcall C_0083ae28_UpdateIfFlagSet(C_0083ae28* self, void* /*edx*/, float a2)
{
    char result = self->field_0038;
    if (result)
        return Autonomous_UpdateBase(self, nullptr, a2);
    return result;
}

HOOK(0x004CEDE0, C_0083ae28_UpdateIfFlagSet, "auto");
