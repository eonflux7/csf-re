#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_TestMovementThreshold, 0x004AA4D0, (void* self, void* edx, float a2));

static bool __fastcall AiCoverAttackTasks_IsMovementBlocked(S_004aa4d0* self, void* /*edx*/)
{
    // Linked object lives at this+8 (the original passes this+8 as ecx).
    uint8_t* linkedSlot = reinterpret_cast<uint8_t*>(self) + 8;
    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedSlot, nullptr);

    // Adjust the returned value by -4 when non-zero, otherwise use null.
    uint8_t* linkedBase = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4))
        : nullptr;

    // linkedBase+0x2c -> A; A+0x3c -> B; B+0x34 holds the float threshold.
    uint8_t* objA = CSF_AT(uint8_t*, linkedBase, 0x2c);
    uint8_t* objB = CSF_AT(uint8_t*, objA, 0x3c);
    float threshold = CSF_AT(float, objB, 0x34);

    return AiCoverAttackTasks_TestMovementThreshold(self, nullptr, threshold);
}

HOOK(0x4AA5A0, AiCoverAttackTasks_IsMovementBlocked, "auto");
