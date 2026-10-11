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

// Callee: Autonomo_DispatchAction, __thiscall-equivalent (self in ecx, a2 on stack, ret 4).
CSF_FUNC(char, __fastcall, Autonomo_DispatchAction, 0x004D1160, (void* self, void* edx, int a2));

// Original: 0x004CEF50, __thiscall, ret 4.
// Returns the flag byte when clear; otherwise tail-forwards the call to Autonomo_DispatchAction.
static char __fastcall C_0083b038_DispatchIfFlagSet(C_0083b038* self, void* /*edx*/, int a2)
{
    char result = self->field_0038;
    if (result)
        return Autonomo_DispatchAction(self, nullptr, a2);
    return result;
}

HOOK(0x004CEF50, C_0083b038_DispatchIfFlagSet, "auto");
