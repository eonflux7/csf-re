#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees, bound by original address.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, TCharTask_CheckRotationGate, 0x0048CBE0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, AiStateAccessors_DispatchLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

// Original: int __stdcall sub_4AB9E0(int a1, int a2), ret 8.
// Resolves the object linked to a1, checks its rotation gate, and if blocked
// dispatches the "no linked target" path on a1.
static int __stdcall AiMoveTasks_DispatchIfRotationGateBlocked(int ownerObject, int unusedArg)
{
    (void)unusedArg;

    void* self = reinterpret_cast<void*>(ownerObject);

    // GetLinkedObjectValue(a1); the result is adjusted by -4 when non-null.
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* linkedBase = nullptr;
    if (linkedValue != 0)
        linkedBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));

    // owner = [linkedBase + 0x2C]; rotationTarget = [owner + 0x204]
    void* linkedOwner = CSF_AT(void*, linkedBase, 0x2C);
    void* rotationTarget = CSF_AT(void*, linkedOwner, 0x204);

    bool rotationGateOpen = TCharTask_CheckRotationGate(rotationTarget, nullptr, 1);
    if (rotationGateOpen)
    {
        return AiStateAccessors_DispatchLinkedTarget(self, nullptr, 0);
    }
    return static_cast<int>(rotationGateOpen);
}

HOOK(0x4AB9E0, AiMoveTasks_DispatchIfRotationGateBlocked, "auto");

