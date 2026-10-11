#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x49EC10 takes the child object in ecx and one float* on the stack.
CSF_FUNC(float*, __fastcall, sub_49EC10, 0x0049EC10, (void* self, void* edx, float* a2));

// __thiscall method: forwards the float* argument to the child object stored at +0x08
// and returns the same pointer.
static float* __fastcall C_0083ec48_ForwardAndReturnArg(C_0083ec48* self, void* /*edx*/, float* a2)
{
    sub_49EC10(reinterpret_cast<void*>(self->field_0008), nullptr, a2);
    return a2;
}

HOOK(0x4E8BA0, C_0083ec48_ForwardAndReturnArg, "auto");
