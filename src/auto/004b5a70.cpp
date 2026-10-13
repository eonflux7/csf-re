#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (S_004abb90* self, void*));
CSF_FUNC(double, __fastcall, ComputeLinkedSubObjectRatio, 0x004A9CD0, (void* self, void*));
CSF_FUNC(char, __fastcall, AiSearch_HasAttackTarget, 0x004B22B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_004af750_TestAttackPosition, 0x004B2E20, (S_004af750* self, void* , float* position, int target));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);
CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

static bool __fastcall S_004abb90_CheckUnassignedSearchCandidate(
    S_004abb90* self, void* /*edx*/, void* candidate)
{
    GetValue8B0554();

    int linked = GetLinkedObjectValue_4DB5C0(candidate, nullptr);
    uintptr_t character = linked ? static_cast<uint32_t>(linked) - 4 : 0;

    if (CSF_AT(int, reinterpret_cast<void*>(character), 0x608) == -1 &&
        CSF_AT(int, reinterpret_cast<void*>(character), 0x60C) == -1)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidate, nullptr);
    character = linked ? static_cast<uint32_t>(linked) - 4 : 0;

    const float candidateX = CSF_AT(float, reinterpret_cast<void*>(character), 0x64C);
    const float candidateY = CSF_AT(float, reinterpret_cast<void*>(character), 0x650);
    const float candidateZ = CSF_AT(float, reinterpret_cast<void*>(character), 0x654);

    auto* searchOrigin = reinterpret_cast<S_004abb90*>(
        reinterpret_cast<uint8_t*>(self) + 0x38);
    const float facingAngle =
        static_cast<float>(ComputeLinkedSubObjectRatio(searchOrigin, nullptr));
    auto* position = reinterpret_cast<float*>(
        AiCoverAttackTasks_GetPositionData(searchOrigin, nullptr));

    alignas(8) uint8_t directionBytes[0x1C];
    auto& direction = *reinterpret_cast<S_00526d40*>(directionBytes);
    const float deltaZ = candidateZ - position[2];
    const float deltaY = candidateY - position[1];
    const float deltaX = candidateX - position[0];
    std::memcpy(&direction.field_0000, &deltaX, sizeof(deltaX));
    CSF_AT(float, directionBytes, 0x4) = deltaY;
    CSF_AT(float, directionBytes, 0x8) = deltaZ;

    const float directionAngle =
        static_cast<float>(S_00526d40_GetVectorAngleRadians(&direction, nullptr));
    const double angleDifference = AngleDiffNormalized(facingAngle, directionAngle);
    if (!(std::fabs(angleDifference) < g_TransitionHalfPi))
        return false;

    if (!self->field_0015)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidate, nullptr);
    character = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    if (CSF_AT(int, reinterpret_cast<void*>(character), 0x608) == -1)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidate, nullptr);
    character = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    uintptr_t target = character +
        (CSF_AT(int, reinterpret_cast<void*>(character), 0x630) < 0
            ? 0x610 : 0x628);
    if (CSF_AT(int, reinterpret_cast<void*>(target), 0x8) < 0)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidate, nullptr);
    character = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    if (!AiSearch_HasAttackTarget(reinterpret_cast<void*>(character + 0x5DC), nullptr))
        return true;

    linked = GetLinkedObjectValue_4DB5C0(candidate, nullptr);
    const uintptr_t targetOwner = linked ? static_cast<uint32_t>(linked) - 4 : 0;

    linked = GetLinkedObjectValue_4DB5C0(candidate, nullptr);
    auto* attacker = linked
        ? reinterpret_cast<S_004af750*>(static_cast<uint32_t>(linked) - 4)
        : nullptr;

    target = targetOwner +
        (CSF_AT(int, reinterpret_cast<void*>(targetOwner), 0x630) < 0
            ? 0x610 : 0x628);
    position = reinterpret_cast<float*>(
        AiCoverAttackTasks_GetPositionData(searchOrigin, nullptr));
    const char canAttack = S_004af750_TestAttackPosition(
        attacker, nullptr, position, static_cast<int>(target));

    g_AiSearchCostThisFrame += 10;
    return canAttack != 0;
}

HOOK(0x004B5A70, S_004abb90_CheckUnassignedSearchCandidate, "auto");
