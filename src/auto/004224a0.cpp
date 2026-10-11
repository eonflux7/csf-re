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

// Callees, bound by address with the signatures from the packet.
CSF_FUNC(char*, __fastcall, StateObj_GetOwnerFlagPtr, 0x004D1510, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, StateObj_GetChildSlot, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiTaskStates_StartTimedAction, 0x00421B30, (void* self, void* edx, float a2, char a3));

// Chooses between two child sub-objects of this state object based on a flag
// read through the owner chain. Flag set: returns the child at +0x194 (the
// result of StateObj_GetChildSlot). Flag clear: fetches the child at +0x1A8,
// starts it with time (+0x240 + +0x23C) and returns the start result.
static int __fastcall S_004223f0_DispatchByOwnerFlag(S_004223f0* self, void* /*edx*/)
{
    uint8_t* state = reinterpret_cast<uint8_t*>(self->field_0008);
    uint32_t owner = CSF_AT(uint32_t, state, 0x14);
    uint32_t ownerFlagHolder = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(owner), 0x34);

    char* ownerFlag = StateObj_GetOwnerFlagPtr(reinterpret_cast<void*>(ownerFlagHolder), nullptr);
    if (*ownerFlag != 0) {
        int childSlotAddr = static_cast<int>(self->field_0008 + 0x194);
        return static_cast<int>(reinterpret_cast<uintptr_t>(
            StateObj_GetChildSlot(self, nullptr, childSlotAddr)));
    }

    int timedChildAddr = static_cast<int>(self->field_0008 + 0x1A8);
    void* timedChild = StateObj_GetChildSlot(self, nullptr, timedChildAddr);

    // Original reloads the state pointer after the call.
    uint8_t* stateAfter = reinterpret_cast<uint8_t*>(self->field_0008);
    float timeValue = CSF_AT(float, stateAfter, 0x240) + CSF_AT(float, stateAfter, 0x23C);

    return AiTaskStates_StartTimedAction(timedChild, nullptr, timeValue, 1);
}

HOOK(0x004224A0, S_004223f0_DispatchByOwnerFlag, "auto");
