#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_GetAngleDifference, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angle));
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* minimum, float* maximum));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0082D754);       // 0.017453292
CSF_GLOBAL(double, g_AngleSettledTolerance, 0x0082D758); // 0.000001
CSF_GLOBAL(float, g_DefaultAngularSpeed, 0x0082D7B0);   // 3.4906585
CSF_GLOBAL(float, g_ActiveAngularSpeed, 0x0082D7B4);    // 5.2359877
CSF_GLOBAL(float, g_InactiveAngularSpeed, 0x0082D7B8);  // 40.142574
CSF_GLOBAL(float, g_DefaultAngleLimit, 0x0082D7BC);     // 0.17453292
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisX_Y, 0x008A7D70);
CSF_GLOBAL(float, g_AxisX_Z, 0x008A7D74);

static float C_0082d77c_GetAngularSpeedLimit(C_0082d77c* self, int channel)
{
    if (channel == 0)
        return g_DefaultAngularSpeed;
    if (channel == 2)
    {
        auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
        return owner->field_0050 ? g_ActiveAngularSpeed : g_InactiveAngularSpeed;
    }
    return g_fZero;
}

static void __fastcall C_0082d77c_UpdateAxisRotation(
    C_0082d77c* self, void* /*edx*/, float elapsedTime, int channel, const float* axis)
{
    auto* record = reinterpret_cast<std::uint8_t*>(self->field_000c) + 44 * channel;
    const bool isXAxis =
        axis[0] == g_AxisX &&
        axis[1] == g_AxisX_Y &&
        axis[2] == g_AxisX_Z;

    float* rotation = reinterpret_cast<float*>(record + (isXAxis ? 12 : 28));
    auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
    void* related = reinterpret_cast<void*>(owner->vGetField016c());
    void* orientation =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, related, 0)[0x18 / 4])(related, nullptr);

    double angleDegrees;
    if (isXAxis)
    {
        angleDegrees =
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, orientation, 0)[0x24 / 4])(orientation, nullptr);
    }
    else
    {
        angleDegrees =
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, orientation, 0)[0x28 / 4])(orientation, nullptr);
    }

    float desiredAngle = static_cast<float>(angleDegrees * g_DegreesToRadians);
    if (channel == 2)
    {
        auto* firstRecord = reinterpret_cast<std::uint8_t*>(self->field_000c);
        if (CSF_AT(std::uint32_t, firstRecord, 0x04) == 1)
        {
            desiredAngle = isXAxis
                ? CSF_AT(float, firstRecord, 0x10)
                : CSF_AT(float, firstRecord, 0x20);
        }
    }

    float targetMovement = 0.0f;
    if (rotation[0] != desiredAngle)
    {
        targetMovement = static_cast<float>(
            std::fabs(MenuMpCharacterSelect_GetAngleDifference(desiredAngle, rotation[0])));
    }

    rotation[0] = desiredAngle;
    float constrainedAngle = rotation[2];
    if (!CSF_AT(std::uint32_t, record, 0x04))
    {
        constrainedAngle = desiredAngle;
    }
    else
    {
        const float difference = static_cast<float>(
            MenuMpCharacterSelect_GetAngleDifference(constrainedAngle, desiredAngle));
        float limit = g_fZero;

        if (channel == 2)
        {
            auto* settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
            limit = CSF_AT(float, settings, 0xAC0) * g_DegreesToRadians;
        }
        else if (channel == 0)
        {
            if (reinterpret_cast<C_00833908*>(self->field_0008)->field_0050)
            {
                auto* settings = reinterpret_cast<std::uint8_t*>(GetGlobalValue_8BC1E0());
                limit = CSF_AT(float, settings, 0xAC0) * g_DegreesToRadians;
            }
            else
            {
                limit = g_DefaultAngleLimit;
            }
        }

        if (difference > limit)
            constrainedAngle = limit + rotation[0];
        else if (-limit > difference)
            constrainedAngle = rotation[0] - limit;
    }

    const float accelerationStep =
        C_0082d77c_GetAngularSpeedLimit(self, channel) * elapsedTime;
    float* speed = rotation + 3;
    const float acceleratedSpeed = accelerationStep + rotation[3];
    rotation[3] = acceleratedSpeed;

    if (acceleratedSpeed > C_0082d77c_GetAngularSpeedLimit(self, channel))
        *speed = 0.0f;

    float maximumSpeed = C_0082d77c_GetAngularSpeedLimit(self, channel);
    float minimumStorage[4] = {};
    ClampFloatToBounds(speed, minimumStorage, &maximumSpeed);

    const float difference = static_cast<float>(
        MenuMpCharacterSelect_GetAngleDifference(constrainedAngle, rotation[1]));
    float movement = elapsedTime * *speed + targetMovement;
    if (difference <= movement)
    {
        movement = -movement;
        if (difference >= movement)
            movement = difference;
    }

    if (std::fabs(static_cast<double>(difference)) < g_AngleSettledTolerance)
    {
        const float reducedSpeed = *speed - accelerationStep * g_MessageBoxAnimationScale;
        *speed = reducedSpeed;
        if (reducedSpeed < 0.0f)
            *speed = 0.0f;
    }

    const float nextAngle = movement + rotation[1];
    rotation[1] = static_cast<float>(MenuMpCharacterSelect_NormalizeAngle(nextAngle));
}

HOOK(0x00407080, C_0082d77c_UpdateAxisRotation, "auto");
