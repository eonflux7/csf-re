#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static float* __fastcall AiAttackTasks_ChooseAimPoint(
    void* /*unused*/, void* /*edx*/, float* output, void* pointSet)
{
    const float heightOffset = CSF_AT(float, pointSet, 0x50);
    const uint32_t beginAddress = CSF_AT(uint32_t, pointSet, 0x24);
    int pointCount = 0;
    if (beginAddress != 0)
    {
        pointCount = static_cast<int32_t>(
            CSF_AT(uint32_t, pointSet, 0x28) - beginAddress) / 12;
    }

    void* state = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalState_50D190()));
    const int selectedIndex = GetStateValue(state, nullptr, pointCount);

    auto* points = CSF_AT(uint8_t*, pointSet, 0x24);
    auto* selected = reinterpret_cast<float*>(points + 12 * selectedIndex);

    int previousIndex = selectedIndex - 1;
    if (previousIndex < 0)
        previousIndex = pointCount - 1;

    int nextIndex = selectedIndex + 1;
    if (nextIndex >= pointCount)
        nextIndex = 0;

    auto* previous = reinterpret_cast<float*>(points + 12 * previousIndex);
    auto* next = reinterpret_cast<float*>(points + 12 * nextIndex);

    float deltaX = previous[0] - next[0];
    float deltaY = previous[1] - next[1];
    float deltaZ = previous[2] - next[2];

    alignas(8) uint8_t directionBytes[0x10]{};
    auto* direction = reinterpret_cast<C_00833b88*>(directionBytes);
    CSF_AT(float, directionBytes, 0x0) = deltaX;
    CSF_AT(float, directionBytes, 0x4) = deltaY;
    CSF_AT(float, directionBytes, 0x8) = deltaZ;
    NormalizeVector3(direction, nullptr);

    int distanceLimit = static_cast<int>(
        static_cast<int64_t>(std::sqrt(
            deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX)));

    state = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalState_50D190()));
    int distance = GetStateValue(state, nullptr, distanceLimit);
    CSF_AT(int, directionBytes, 0xC) = distance;

    double scale = static_cast<double>(distance);
    if (distance < 0)
        scale += g_UnsignedIntegerBias;

    const float scaledY = static_cast<float>(
        CSF_AT(float, directionBytes, 0x4) * scale);
    const float scaledZ = static_cast<float>(
        CSF_AT(float, directionBytes, 0x8) * scale);
    const double sampleX = CSF_AT(float, directionBytes, 0x0) * scale + next[0];
    const float sampleY = scaledY + next[1];
    const float sampleZ = scaledZ + next[2];

    deltaX = static_cast<float>(sampleX - selected[0]);
    deltaY = sampleY - selected[1];
    deltaZ = sampleZ - selected[2];

    CSF_AT(float, directionBytes, 0x0) = deltaX;
    CSF_AT(float, directionBytes, 0x4) = deltaY;
    CSF_AT(float, directionBytes, 0x8) = deltaZ;
    NormalizeVector3(direction, nullptr);

    distanceLimit = static_cast<int>(
        static_cast<int64_t>(std::sqrt(
            deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX)));

    state = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalState_50D190()));
    distance = GetStateValue(state, nullptr, distanceLimit);
    CSF_AT(int, directionBytes, 0xC) = distance;

    scale = static_cast<double>(distance);
    if (distance < 0)
        scale += g_UnsignedIntegerBias;

    const float finalScaledY = static_cast<float>(
        CSF_AT(float, directionBytes, 0x4) * scale);
    const float finalScaledZ = static_cast<float>(
        CSF_AT(float, directionBytes, 0x8) * scale);
    const double resultX = CSF_AT(float, directionBytes, 0x0) * scale + selected[0];
    const float resultY = finalScaledY + selected[1];
    const float resultZ = finalScaledZ + selected[2];

    output[0] = static_cast<float>(resultX);
    output[1] = resultY + heightOffset;
    output[2] = resultZ;
    return output;
}

HOOK(0x0047C7A0, AiAttackTasks_ChooseAimPoint, "auto");
