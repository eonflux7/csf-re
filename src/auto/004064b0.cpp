#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x4059D0 (__thiscall, one stack arg, ret 4).
CSF_FUNC(char, __fastcall, sub_4059D0, 0x004059D0, (void* self, void* edx, int a2));

// __thiscall C_0082d70c::? (ecx = this, one stack arg, ret 4).
// Forwards to the base-class routine at 0x4059D0; on success, resets two state fields.
static char __fastcall C_0082d70c_ForwardAndResetOnSuccess(C_0082d70c* self, void* /*edx*/, int a2)
{
    char result = sub_4059D0(self, nullptr, a2);
    if (result) {
        CSF_AT(int, self, 0x3C) = 0x40000000;
        CSF_AT(int, self, 0x40) = 0;
    }
    return result;
}

HOOK(0x004064B0, C_0082d70c_ForwardAndResetOnSuccess, "auto");
