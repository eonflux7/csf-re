#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (S_004abb90* self, void*));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, S_004af750_CheckSlotPosition, 0x004B2EE0, (S_004af750* self, void* , float* position, int slot));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

static char __fastcall S_004abb90_ValidateSearchCandidate(
    S_004abb90* self, void* /*edx*/, int candidate)
{
    void* candidateHandle = reinterpret_cast<void*>(candidate);

    uintptr_t linked = static_cast<uintptr_t>(
        GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr));
    uintptr_t candidateBase = linked ? linked - 4 : 0;

    uintptr_t slotAddress = candidateBase +
        (CSF_AT(int, reinterpret_cast<void*>(candidateBase), 0x630) < 0
            ? 0x610 : 0x628);
    if (CSF_AT(int, reinterpret_cast<void*>(slotAddress), 8) < 0)
        return 1;

    linked = static_cast<uintptr_t>(
        GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr));
    candidateBase = linked ? linked - 4 : 0;

    void* coverState = reinterpret_cast<void*>(candidateBase + 0x5DC);
    slotAddress = reinterpret_cast<uintptr_t>(coverState) +
        (CSF_AT(int, coverState, 0x54) < 0 ? 0x34 : 0x4C);
    if (!CoverAttack_CheckState_4ADAE0(
            coverState, nullptr, static_cast<int>(slotAddress)))
        return 1;

    linked = static_cast<uintptr_t>(
        GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr));
    uintptr_t slotOwnerBase = linked ? linked - 4 : 0;

    linked = static_cast<uintptr_t>(
        GetLinkedObjectValue_4DB5C0(candidateHandle, nullptr));
    S_004af750* positionChecker =
        reinterpret_cast<S_004af750*>(linked ? linked - 4 : 0);

    slotAddress = slotOwnerBase +
        (CSF_AT(int, reinterpret_cast<void*>(slotOwnerBase), 0x630) < 0
            ? 0x610 : 0x628);

    float* position = reinterpret_cast<float*>(
        AiCoverAttackTasks_GetPositionData(
            reinterpret_cast<S_004abb90*>(
                reinterpret_cast<uint8_t*>(self) + 0x38),
            nullptr));

    char result = S_004af750_CheckSlotPosition(
        positionChecker, nullptr, position, static_cast<int>(slotAddress));
    g_AiSearchCostThisFrame += 10;
    return result;
}

HOOK(0x004B5C30, S_004abb90_ValidateSearchCandidate, "auto");
