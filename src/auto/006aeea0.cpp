#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall AiSubStateObject_TestReady(float* this): returns 1 when the float at +0x4C
// is not less than the float at +0x48, otherwise 0.
static int __fastcall AiSubStateObject_TestReady(void* self, void* /*edx*/)
{
    float valueAt4C = CSF_AT(float, self, 0x4C);
    float valueAt48 = CSF_AT(float, self, 0x48);
    return valueAt4C >= valueAt48 ? 1 : 0;
}

HOOK(0x6AEEA0, AiSubStateObject_TestReady, "auto");
