#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original constant 0.0f used by the null path (fld dword ptr [0x82D4CC]).
CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Returns the state value of the task-state object at this+0x48, delegating
// to the object's vtable slot 0x44 when present. Otherwise returns 0.0.
static hook::Tail __fastcall AiTaskStates_ComputeStateValue(void* self, const uint32_t* args) {
    (void)args;  // The jump target receives the caller's stack args untouched.

    // Original: mov eax, [ecx+0x48]
    const uint32_t stateObjectAddr = CSF_AT(uint32_t, self, 0x48);
    void* stateObject = reinterpret_cast<void*>(stateObjectAddr);

    if (stateObjectAddr == 0) {
        // Original: fld dword ptr [g_fZero]; the result is 0.0.
        // eax is 0 on this path (the null test just passed).
        return hook::tail_ret(0u, 4);
    }

    // Original: jmp dword ptr [eax + 0x44] on the vtable of the state object.
    return hook::tail_jump(stateObject, (*reinterpret_cast<void***>(stateObject))[0x44 / 4]);
}

HOOK_TAIL(0x0048E510, AiTaskStates_ComputeStateValue, "auto");
