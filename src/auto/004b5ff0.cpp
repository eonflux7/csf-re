#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00835440);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall AiFncSearch_UpdateFacingAwayCount(
    void* self, void* /*edx*/, void* linkedObject)
{
    std::uintptr_t linkedValue =
        static_cast<std::uintptr_t>(GetLinkedObjectValue_4DB5C0(linkedObject, nullptr));
    auto* actor = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);

    if (CSF_AT(int, actor, 0x608) == -1)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        return;
    }

    linkedValue =
        static_cast<std::uintptr_t>(GetLinkedObjectValue_4DB5C0(linkedObject, nullptr));
    actor = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);

    auto* coverState = actor + 0x5DC;
    auto* stateHandle = coverState +
        (CSF_AT(int, coverState, 0x54) < 0 ? 0x34 : 0x4C);

    if (!CoverAttack_CheckState_4ADAE0(
            coverState, nullptr, reinterpret_cast<int>(stateHandle)))
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        return;
    }

    linkedValue =
        static_cast<std::uintptr_t>(GetLinkedObjectValue_4DB5C0(linkedObject, nullptr));
    actor = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);

    auto* targetHandle = actor +
        (CSF_AT(int, actor, 0x630) < 0 ? 0x610 : 0x628);
    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                             reinterpret_cast<int>(targetHandle)));

    if (!target || target->field_0050)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        return;
    }

    const float targetAngle = static_cast<float>(
        C_008342e8_ComputeResolvedObjectScalar(target, nullptr) *
        g_DegreesToRadians);

    linkedValue =
        static_cast<std::uintptr_t>(GetLinkedObjectValue_4DB5C0(linkedObject, nullptr));
    actor = reinterpret_cast<std::uint8_t*>(linkedValue ? linkedValue - 4 : 0);
    auto* actorObject = CSF_AT(C_008342e8*, actor, 0x2C);

    const float* targetPosition = GetObjectPositionVector(target, nullptr);
    const float* actorPosition = GetObjectPositionVector(actorObject, nullptr);

    const float deltaZ = actorPosition[2] - targetPosition[2];
    const float deltaY = actorPosition[1] - targetPosition[1];
    const float deltaX = actorPosition[0] - targetPosition[0];

    alignas(8) std::uint8_t directionBytes[0xC];
    auto& direction = *reinterpret_cast<S_00526d40*>(directionBytes);
    std::memcpy(&direction.field_0000, &deltaX, sizeof(deltaX));
    CSF_AT(float, directionBytes, 0x4) = deltaY;
    CSF_AT(float, directionBytes, 0x8) = deltaZ;

    const float directionAngle = static_cast<float>(
        S_00526d40_GetVectorAngleRadians(&direction, nullptr));
    const double angleDifference = AngleDiffNormalized(targetAngle, directionAngle);

    if (!(std::fabs(angleDifference) > g_TransitionHalfPi))
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        return;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x18) + 1;
    const std::uint32_t threshold = CSF_AT(std::uint32_t, self, 0x14);
    CSF_AT(std::uint32_t, self, 0x18) = count;
    if (count >= threshold)
        CSF_AT(int, self, 0x4) = 3;
}

HOOK(0x004B5FF0, AiFncSearch_UpdateFacingAwayCount, "auto");
