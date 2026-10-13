#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCombat_BindCombatTarget, 0x004B2190, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, S_004bd310_EvaluateCoverTarget, 0x004C4490, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (C_0083e9d0* self, void*));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(int, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (C_0083e9d0* self, void* , int a2));

static bool __fastcall S_004bd310_TryBindCoverTarget(
    S_004bd310* self, void* /*edx*/, int actor, int target)
{
    alignas(8) uint8_t combatTargetBytes[0x34];
    alignas(8) uint8_t endpointBytes[0x14];

    CSF_AT(void**, combatTargetBytes, 0x0C) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, combatTargetBytes, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, combatTargetBytes, 0x18) = 0;
    CSF_AT(int, combatTargetBytes, 0x1C) = 0;
    CSF_AT(int, combatTargetBytes, 0x20) = 0;
    CSF_AT(int, combatTargetBytes, 0x00) = -1;
    CSF_AT(int, combatTargetBytes, 0x04) = -1;
    CSF_AT(int, combatTargetBytes, 0x08) = -1;
    CSF_AT(int, combatTargetBytes, 0x10) = -1;
    CSF_AT(int, combatTargetBytes, 0x14) = -1;
    CSF_AT(int, combatTargetBytes, 0x28) = -1;
    CSF_AT(int, combatTargetBytes, 0x2C) = -1;
    CSF_AT(int, combatTargetBytes, 0x30) = 0;

    AiCombat_BindCombatTarget(combatTargetBytes, nullptr, actor, target);
    PairLink_PrepareForRebind(
        CSF_AT(C_0083e9d0*, reinterpret_cast<void*>(self->field_0000), 0x3C),
        nullptr);

    const int endpointFirst = CSF_AT(int, reinterpret_cast<void*>(target), 0x04);
    const int endpointSecond = CSF_AT(int, reinterpret_cast<void*>(target), 0x08);
    CSF_AT(int, endpointBytes, 0x0C) = endpointFirst;
    CSF_AT(int, endpointBytes, 0x00) = -1;
    CSF_AT(int, endpointBytes, 0x04) = -1;
    CSF_AT(void**, endpointBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, endpointBytes, 0x10) = endpointSecond;

    if (static_cast<uint8_t>(PairLink_TestEndpoints(
            CSF_AT(C_0083e9d0*, reinterpret_cast<void*>(self->field_0000), 0x3C),
            nullptr, reinterpret_cast<int>(endpointBytes))) != 0)
    {
        S_004bd310_EvaluateCoverTarget(
            self, nullptr, reinterpret_cast<uint32_t*>(combatTargetBytes));
    }

    DecrementByteCounter(
        CSF_AT(C_0083e9d0*, reinterpret_cast<void*>(self->field_0000), 0x3C),
        nullptr);
    return CSF_AT(int, combatTargetBytes, 0x2C) >= 0;
}

HOOK(0x004C4500, S_004bd310_TryBindCoverTarget, "auto");
