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

// Callee: TCharTask forwarder, tail-jumped with self in ecx and a2 on the stack (ret 4 popped by target).
CSF_FUNC(char, __fastcall, TCharTask_ForwardUpdateToChild, 0x004DB0F0, (void* self, void* edx, int a2));

// Original: 0x004D1160, __thiscall, ret 4.
// Returns the flag byte when clear; otherwise tail-forwards the call to TCharTask_ForwardUpdateToChild.
static char __fastcall C_0083b958_DispatchAction(C_0083b958* self, void* /*edx*/, int a2)
{
    char result = self->field_0038;
    if (result)
        return TCharTask_ForwardUpdateToChild(self, nullptr, a2);
    return result;
}

HOOK(0x004D1160, C_0083b958_DispatchAction, "auto");
