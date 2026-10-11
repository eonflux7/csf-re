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

// Callee: char __usercall(int a1 @ecx, long double a2 @st0). It takes no stack
// arguments. The float argument travels in st0, which C++ cannot name here, so
// the bind takes only ecx and the wrapper leaves the x87 stack untouched. st0
// then reaches the callee unchanged.
CSF_FUNC(char, __fastcall, AiTaskStates_DispatchByStateValue, 0x422190, (int stateValue, void* /*edx*/));

// Loads the dword at this+8 into ecx and forwards to 0x422190.
static char __fastcall AiTaskStates_DispatchWithFieldAt8(void* self, void* /*edx*/)
{
    int stateValue = CSF_AT(int, self, 0x8);
    return AiTaskStates_DispatchByStateValue(stateValue, nullptr);
}

HOOK(0x4225E0, AiTaskStates_DispatchWithFieldAt8, "auto");
