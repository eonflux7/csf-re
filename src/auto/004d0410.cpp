#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00862490_GetPosition, 0x004760B0, (C_00862490* self, void* , std::uint32_t* position));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_NormalizeCopyAndReportY, 0x006AB9E0, (S_00526d40* self, void*));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_AngleScale, 0x0083B644);

static bool __fastcall C_0083dd58_IsTargetWithinSensorLimits(
    C_0083dd58* self, void* /*edx*/, int targetHandle, int sensorId)
{
    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle));
    if (!target)
        return false;

    auto* owner = reinterpret_cast<C_008342e8*>(self->field_0014);
    void* sensors = reinterpret_cast<void*>(owner->field_0044);
    if (!sensors)
        return false;

    int sensorIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, sensors, 0)[0xBC / 4])(sensors, nullptr, sensorId);
    if (sensorIndex == -1)
        return false;

    auto* sensor =
        reinterpret_cast<C_00862490*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, sensors, 0)[0xCC / 4])(sensors, nullptr, sensorIndex);
    if (!sensor)
        return false;

    auto* settings = reinterpret_cast<std::uint8_t*>(
        Handle_GetValuePointer(sensor, nullptr)) + 4;

    alignas(8) std::uint8_t sensorPositionBytes[0xC] = {};
    C_00862490_GetPosition(
        sensor, nullptr, reinterpret_cast<std::uint32_t*>(sensorPositionBytes));

    auto* position = reinterpret_cast<float*>(
        GetObjectPositionVector(target, nullptr));
    float targetX = position[0];
    float targetY = position[1];
    float targetZ = position[2];

    alignas(8) std::uint8_t targetPositionBytes[0xC];
    if ((target->field_0170 & 0x2000) != 0)
    {
        position = reinterpret_cast<float*(__fastcall*)(void*, void*, void*)>(
            target->vtbl[0xA4 / 4])(target, nullptr, targetPositionBytes);
        targetX = position[0];
        targetY = position[1];
        targetZ = position[2];
    }

    alignas(8) std::uint8_t displacementBytes[0x1C] = {};
    S_00526d40& displacement =
        *reinterpret_cast<S_00526d40*>(displacementBytes);
    CSF_AT(float, displacementBytes, 0x0) =
        targetX - CSF_AT(float, sensorPositionBytes, 0x0);
    CSF_AT(float, displacementBytes, 0x4) =
        targetY - CSF_AT(float, sensorPositionBytes, 0x4);
    CSF_AT(float, displacementBytes, 0x8) =
        targetZ - CSF_AT(float, sensorPositionBytes, 0x8);

    const float range = CSF_AT(float, settings, 0xE8);
    const float dx = CSF_AT(float, displacementBytes, 0x0);
    const float dy = CSF_AT(float, displacementBytes, 0x4);
    const float dz = CSF_AT(float, displacementBytes, 0x8);
    const float distanceSquared = dy * dy + dx * dx + dz * dz;

    // The original's 0x4100 test rejects equality and unordered comparisons.
    const unsigned rangeFailure =
        range * range > distanceSquared ? 0 : 0x4100;
    if (rangeFailure != 0)
        return false;

    const float horizontalAngle = CSF_AT(float, settings, 0xEC) * g_AngleScale;
    const float verticalLimit = CSF_AT(float, settings, 0xF0) * g_AngleScale;

    const float cosineAngle = static_cast<float>(
        C_008342e8_ComputeResolvedObjectScalar(
            reinterpret_cast<C_008342e8*>(self->field_0014), nullptr)
        * g_AngleScale);
    const double sineAngle = C_008342e8_ComputeResolvedObjectScalar(
        reinterpret_cast<C_008342e8*>(self->field_0014), nullptr)
        * g_AngleScale;

    alignas(8) std::uint8_t horizontalDirectionBytes[0x8];
    CSF_AT(float, horizontalDirectionBytes, 0x0) =
        CSF_AT(float, displacementBytes, 0x0);
    CSF_AT(float, horizontalDirectionBytes, 0x4) =
        CSF_AT(float, displacementBytes, 0x8);

    const float headingSin = static_cast<float>(std::sin(sineAngle));
    const float headingCos = std::cos(cosineAngle);
    NormalizeVector2(horizontalDirectionBytes, nullptr);

    const float alignment =
        CSF_AT(float, horizontalDirectionBytes, 0x4) * headingCos
        + CSF_AT(float, horizontalDirectionBytes, 0x0) * headingSin;
    if (!(std::cos(horizontalAngle) <= alignment))
        return false;

    return std::fabs(
        S_00526d40_NormalizeCopyAndReportY(&displacement, nullptr)) < verticalLimit;
}

HOOK(0x004D0410, C_0083dd58_IsTargetWithinSensorLimits, "auto");
