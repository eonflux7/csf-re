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

// Callee: C_0082e520::UpdateFromEvent, __thiscall-equivalent (self in ecx, a2 on stack, ret 4).
CSF_FUNC(char, __fastcall, C_0082e520_UpdateFromEvent, 0x0041E090, (C_0082e520* self, void* , int));

// Original: 0x0040F700, __thiscall, ret 4.
// Returns the flag byte when clear; otherwise tail-forwards the call to C_0082e520_UpdateFromEvent.
static char __fastcall C_0082dc10_ForwardIfFlagSet(C_0082dc10* self, void* /*edx*/, int a2)
{
    char result = self->field_000c;
    if (result)
        return C_0082e520_UpdateFromEvent(reinterpret_cast<C_0082e520*>(self), nullptr, a2);
    return result;
}

HOOK(0x0040F700, C_0082dc10_ForwardIfFlagSet, "auto");
