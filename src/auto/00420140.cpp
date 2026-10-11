#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callee at 0x412080: __thiscall (ecx = this), one float stack argument, ret 4.
CSF_FUNC(void, __fastcall, AiTaskStates_BeginPendingMovement, 0x00412080, (void* self, void* edx, float a2));

// Sets the effect intensity on the owner object if its 0x1c4 flag is clear, then marks this->0x24.
// Returns the owner pointer loaded from this+0x14 (the original's eax at entry).
static int __fastcall AiTaskStates_SetEffectIntensity(void* self, void* /*edx*/, float a2)
{
    int result = CSF_AT(int, self, 0x14);
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));

    if (CSF_AT(int, owner, 0x1c4) == 0)
    {
        uint8_t* movementChild = CSF_AT(uint8_t*, owner, 0x4c);
        AiTaskStates_BeginPendingMovement(movementChild, nullptr, a2);
        CSF_AT(int, self, 0x24) = 1;
    }
    return result;
}

HOOK(0x420140, AiTaskStates_SetEffectIntensity, "auto");
