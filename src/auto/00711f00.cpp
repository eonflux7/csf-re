#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_WheelRecordScale, 0x0082DCE8);
CSF_GLOBAL(float, g_ScaleY_0_6, 0x0082E038);
CSF_GLOBAL(float, g_FacingAngleOffset, 0x0082E044);
CSF_GLOBAL(float, g_AudioAttenuationDistance, 0x00830E48);
CSF_GLOBAL(float, g_AudioRadiansToDegrees, 0x00863D34);
CSF_GLOBAL(float, g_AudioPanAngleScale, 0x00863DC8);
CSF_GLOBAL(float, g_AudioInverseAttenuationDistance, 0x00863DCC);

static int __fastcall C_00863d80_UpdateSpatialPanAndVolume(
    C_00863d80* self, void* /*edx*/, float* position)
{
    alignas(8) uint8_t listenerPositionBytes[0xC];
    alignas(8) uint8_t listenerDirectionBytes[0xC];
    alignas(8) uint8_t displacementBytes[0x10];

    void* listener = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    float* listenerPosition =
        reinterpret_cast<float* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, listener, 0)[0x44 / 4])(
                listener, nullptr, listenerPositionBytes);

    const float deltaZ = position[2] - listenerPosition[2];
    const float deltaY = position[1] - listenerPosition[1];
    const float deltaX = position[0] - listenerPosition[0];
    CSF_AT(float, displacementBytes, 0x0) = deltaX;
    CSF_AT(float, displacementBytes, 0x4) = deltaY;
    CSF_AT(float, displacementBytes, 0x8) = deltaZ;

    listener = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, listener, 0)[0x4C / 4])(
            listener, nullptr, listenerDirectionBytes);

    CSF_AT(float, displacementBytes, 0xC) = 1.0f;
    const float distance = std::sqrt(
        CSF_AT(float, displacementBytes, 0x0) *
            CSF_AT(float, displacementBytes, 0x0) +
        CSF_AT(float, displacementBytes, 0x8) *
            CSF_AT(float, displacementBytes, 0x8) +
        CSF_AT(float, displacementBytes, 0x4) *
            CSF_AT(float, displacementBytes, 0x4));

    if (distance > g_fZero)
    {
        if (distance >= g_AudioAttenuationDistance)
            CSF_AT(float, displacementBytes, 0xC) = 0.40000001f;
        else
            CSF_AT(float, displacementBytes, 0xC) =
                (g_AudioAttenuationDistance - distance) *
                g_AudioInverseAttenuationDistance *
                g_ScaleY_0_6 + g_WheelRecordScale;
    }

    CSF_AT(float, displacementBytes, 0x4) = 0.0f;
    CSF_AT(uint32_t, listenerDirectionBytes, 0x4) = 0;

    S_00526d40& displacement =
        *reinterpret_cast<S_00526d40*>(displacementBytes);
    S_00526d40& listenerDirection =
        *reinterpret_cast<S_00526d40*>(listenerDirectionBytes);

    const float displacementAngle = static_cast<float>(
        S_00526d40_GetVectorAngleRadians(&displacement, nullptr));
    const float listenerAngle = static_cast<float>(
        S_00526d40_GetVectorAngleRadians(&listenerDirection, nullptr));

    const float angleDegrees = static_cast<float>(
        AngleDiffNormalized(listenerAngle, displacementAngle) *
        g_AudioRadiansToDegrees);
    const float sign = angleDegrees < g_fZero
        ? g_MissingMetricValue : g_TransitionOne;

    float panAngle = angleDegrees * sign;
    if (panAngle > g_FacingAngleOffset)
        panAngle -= g_FacingAngleOffset;

    void* sound = reinterpret_cast<void*>(self->field_000c);
    const float panMagnitude = panAngle * g_AudioPanAngleScale;
    if (!sound)
        return 0;

    reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, sound, 0)[0x34 / 4])(
            sound, nullptr, panMagnitude * sign);

    sound = reinterpret_cast<void*>(self->field_000c);
    return reinterpret_cast<int (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, sound, 0)[0x38 / 4])(
            sound, nullptr, CSF_AT(float, displacementBytes, 0xC));
}

HOOK(0x00711F00, C_00863d80_UpdateSpatialPanAndVolume, "auto");
