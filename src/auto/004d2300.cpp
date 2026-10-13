#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionY, 0x004D1B50, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));
CSF_FUNC(double, __stdcall, WrapFacingAngleDegrees, 0x004D2100, (float a1));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(double, __fastcall, NetCogibleDDL_GetWrappedAngleDegrees, 0x00740AA0, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_FacingAngleOffset, 0x0082E044);
CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_FullRotationDegrees, 0x00830D04);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x00830D08);
CSF_GLOBAL(float, g_RadiansToFacingDegrees, 0x0083BA1C);
CSF_GLOBAL(float, g_PositiveFacingLimit, 0x0083BB04);
CSF_GLOBAL(float, g_NegativeFacingLimit, 0x0083BB08);
CSF_GLOBAL(float, g_RightStrafeUpperAngle, 0x0083BB0C);
CSF_GLOBAL(float, g_LeftStrafeUpperAngle, 0x0083BB10);
CSF_GLOBAL(float, g_ListenerOffsetScale_83BB14, 0x0083BB14);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);

static void __fastcall UpdatePlayerFacing(
    void* self, void* /*edx*/, CMenuMpStats* player, float elapsedTime)
{
    alignas(8) uint8_t firstAxisBytes[0xC];
    alignas(8) uint8_t secondAxisBytes[0xC];
    alignas(8) uint8_t displacementBytes[0x10] = {};
    S_00526d40& displacement =
        *reinterpret_cast<S_00526d40*>(displacementBytes);

    const float playerZ = static_cast<float>(GetPlayerPositionZ(player, nullptr));
    const float playerY = static_cast<float>(GetPlayerPositionY(player, nullptr));
    const float dx = static_cast<float>(
        GetPlayerPositionX(player, nullptr) - CSF_AT(float, self, 0x4C));
    std::memcpy(&displacement.field_0000, &dx, sizeof(dx));
    CSF_AT(float, displacementBytes, 0x4) =
        playerY - CSF_AT(float, self, 0x50);
    const float dz = playerZ - CSF_AT(float, self, 0x54);
    CSF_AT(uint8_t, self, 0x65) = 0;
    CSF_AT(uint8_t, self, 0x66) = 0;
    CSF_AT(float, displacementBytes, 0x8) = dz;

    const float dy = CSF_AT(float, displacementBytes, 0x4);
    if (dz * dz + dy * dy + dx * dx > g_ControlHoldThreshold)
    {
        const float playerAngle = static_cast<float>(
            NetCogibleDDL_GetWrappedAngleDegrees(player, nullptr));
        double movementAngle = WrapFacingAngleDegrees(static_cast<float>(
            S_00526d40_GetVectorAngleRadians(&displacement, nullptr) *
            g_RadiansToFacingDegrees));
        const double wrappedPlayerAngle = WrapFacingAngleDegrees(playerAngle);
        const float relativeAngle = static_cast<float>(
            WrapFacingAngleDegrees(
                static_cast<float>(movementAngle - wrappedPlayerAngle)));

        if (relativeAngle >= g_ListenerOffsetScale_83BB14)
        {
            if (relativeAngle >= g_LeftStrafeUpperAngle)
            {
                if (relativeAngle >= g_ColorChannelScale)
                {
                    if (relativeAngle < g_RightStrafeUpperAngle)
                    {
                        movementAngle += g_FacingAngleOffset;
                        CSF_AT(uint8_t, self, 0x66) = 1;
                    }
                }
                else
                {
                    movementAngle += g_PositiveHalfRotationDegrees;
                }
            }
            else
            {
                movementAngle -= g_FacingAngleOffset;
                CSF_AT(uint8_t, self, 0x65) = 1;
            }
        }

        const float targetAngle = static_cast<float>(
            WrapFacingAngleDegrees(static_cast<float>(movementAngle)));

        void* object = CSF_AT(void*, self, 0x14);
        void* resolved =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
        void* controller =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, resolved, 0)[0x68 / 4])(resolved, nullptr);

        double angleDifference = WrapFacingAngleDegrees(
            targetAngle - CSF_AT(float, self, 0x80));
        if (angleDifference > g_PositiveHalfRotationDegrees)
            angleDifference -= g_FullRotationDegrees;

        if (angleDifference < g_fZero)
        {
            const double newAngle = WrapFacingAngleDegrees(
                CSF_AT(float, self, 0x80) -
                elapsedTime * g_FullRotationDegrees);
            CSF_AT(float, self, 0x80) = static_cast<float>(newAngle);
            double remaining = WrapFacingAngleDegrees(
                static_cast<float>(targetAngle - newAngle));
            if (remaining > g_PositiveHalfRotationDegrees)
                remaining -= g_FullRotationDegrees;
            if (remaining > g_fZero)
                CSF_AT(float, self, 0x80) = targetAngle;
        }
        else if (angleDifference > g_fZero)
        {
            const double newAngle = WrapFacingAngleDegrees(
                elapsedTime * g_FullRotationDegrees +
                CSF_AT(float, self, 0x80));
            CSF_AT(float, self, 0x80) = static_cast<float>(newAngle);
            double remaining = WrapFacingAngleDegrees(
                static_cast<float>(targetAngle - newAngle));
            if (remaining > g_PositiveHalfRotationDegrees)
                remaining -= g_FullRotationDegrees;
            if (remaining < g_fZero)
                CSF_AT(float, self, 0x80) = targetAngle;
        }

        if (CSF_AT(uint8_t, self, 0x66) ||
            CSF_AT(uint8_t, self, 0x65))
        {
            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, controller, 0)[0x6C / 4])(
                    controller, nullptr, CSF_AT(uint32_t, self, 0xB4));
            void* firstPart =
                ((void*(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, controller, 0)[0x70 / 4])(
                        controller, nullptr, CSF_AT(uint32_t, self, 0xB4));
            ((void(__fastcall*)(void*, void*, void*, float*))
                CSF_AT(void**, firstPart, 0)[0x30 / 4])(
                    firstPart, nullptr, secondAxisBytes, &g_AxisY);

            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, controller, 0)[0x6C / 4])(
                    controller, nullptr, CSF_AT(uint32_t, self, 0xA8));
            void* secondPart =
                ((void*(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, controller, 0)[0x70 / 4])(
                        controller, nullptr, CSF_AT(uint32_t, self, 0xA8));
            ((void(__fastcall*)(void*, void*, void*, float*))
                CSF_AT(void**, secondPart, 0)[0x30 / 4])(
                    secondPart, nullptr, firstAxisBytes, &g_AxisY);
        }

        const double facing = C_008342e8_ComputeResolvedObjectScalar(
            CSF_AT(C_008342e8*, self, 0x14), nullptr);
        CSF_AT(float, self, 0x7C) = static_cast<float>(facing);
        CSF_AT(float, self, 0x78) = static_cast<float>(facing);
        return;
    }

    const double objectFacing = C_008342e8_ComputeResolvedObjectScalar(
        CSF_AT(C_008342e8*, self, 0x14), nullptr);
    double difference = WrapFacingAngleDegrees(static_cast<float>(
        objectFacing - CSF_AT(float, self, 0x7C)));
    if (difference >= g_PositiveHalfRotationDegrees)
        difference -= g_FullRotationDegrees;

    if (difference <= g_fZero)
    {
        if (difference < g_NegativeFacingLimit)
        {
            CSF_AT(float, self, 0x7C) = static_cast<float>(
                C_008342e8_ComputeResolvedObjectScalar(
                    CSF_AT(C_008342e8*, self, 0x14), nullptr) +
                g_PositiveFacingLimit - g_TransitionOne);
        }
        else if (difference < g_fReturnedConstant_0082E030)
        {
            CSF_AT(float, self, 0x7C) =
                CSF_AT(float, self, 0x7C) - elapsedTime * g_FacingAngleOffset;
        }
    }
    else
    {
        if (difference > g_PositiveFacingLimit)
        {
            CSF_AT(float, self, 0x7C) = static_cast<float>(
                C_008342e8_ComputeResolvedObjectScalar(
                    CSF_AT(C_008342e8*, self, 0x14), nullptr) -
                g_PositiveFacingLimit + g_TransitionOne);
        }
        else if (difference > g_RotationWrapStep)
        {
            CSF_AT(float, self, 0x7C) =
                elapsedTime * g_FacingAngleOffset + CSF_AT(float, self, 0x7C);
        }
    }

    const double normalizedFacing =
        WrapFacingAngleDegrees(CSF_AT(float, self, 0x7C));
    CSF_AT(float, self, 0x7C) = static_cast<float>(normalizedFacing);
    CSF_AT(float, self, 0x80) = static_cast<float>(normalizedFacing);
    if (normalizedFacing > g_PositiveHalfRotationDegrees)
        CSF_AT(float, self, 0x7C) =
            static_cast<float>(normalizedFacing - g_FullRotationDegrees);

    const double turn = WrapFacingAngleDegrees(
        CSF_AT(float, self, 0x78) - CSF_AT(float, self, 0x7C));
    const double scaledTurn = turn * (CSF_AT(uint8_t, self, 0x42)
        ? CSF_AT(float, self, 0x74)
        : CSF_AT(float, self, 0x70));
    if (scaledTurn != g_fZero)
    {
        void* object = CSF_AT(void*, self, 0x14);
        CSF_AT(float, CSF_AT(void*, object, 0x48), 0x7C) =
            static_cast<float>(scaledTurn);
    }

    void* object = CSF_AT(void*, self, 0x14);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, object, 0)[0x8C / 4])(
            object, nullptr, 14, 0, -1, 0);
}

HOOK(0x004D2300, UpdatePlayerFacing, "auto");
