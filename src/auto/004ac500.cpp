#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bindings for the original callees (copied from the packet).
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Returns true (1) when the linked object's state at +0x1C4 is not 1, and resets the
// linked cover-attack task state; returns false (0) otherwise.
static char __stdcall AiCoverTasks_ResetTaskStateIfNotActive(int self)
{
    char resetPerformed = 0;

    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(self), nullptr);
    int linkedBase = linkedValue ? linkedValue - 4 : 0;

    // linkedBase->+0x2C is a pointer; its +0x1C4 field is the state value.
    int* statePtrHolder = reinterpret_cast<int*>(static_cast<uintptr_t>(linkedBase) + 0x2C);
    uint8_t* stateObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*statePtrHolder));
    int stateValue = CSF_AT(int, stateObject, 0x1C4);

    if (stateValue != 1)
    {
        resetPerformed = 1;
        int linkedField10 = GetLinkedObjectField10(reinterpret_cast<void*>(self), nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(reinterpret_cast<void*>(self), nullptr, linkedField10 + 16);
    }

    return resetPerformed;
}

HOOK(0x004AC500, AiCoverTasks_ResetTaskStateIfNotActive, "auto");
