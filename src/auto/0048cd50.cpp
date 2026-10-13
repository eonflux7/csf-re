#include <cstdint>
#include "globals.h"
#include <cmath>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(void, __fastcall, ApplyTargetPointRequest, 0x0040FC50, (void* self, void* , void* request));
CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* , float a2, char a3));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_CharacterHeightScale, 0x0082D4BC);
CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00833830);
CSF_GLOBAL(float, g_PreviewRightScale, 0x00833850);

static void __fastcall UpdateTargetFacingAndPointRequests(
    void* self, void* /*edx*/, int /*unused*/, float elapsedTime)
{
    if ((CSF_AT(uint8_t, self, 0x39) & 3) == 0)
        return;

    const bool updateFacing = CSF_AT(int16_t, self, 0x20) > 0;
    CSF_AT(float, self, 0x3C) = elapsedTime + CSF_AT(float, self, 0x3C);

    alignas(8) uint8_t requestBytes[0x20] = {};
    alignas(8) uint8_t positionBytes[0xC] = {};
    alignas(8) uint8_t angleBytes[0xC] = {};
    alignas(8) uint8_t correctionBytes[0x4] = {};

    if (updateFacing)
    {
        void* owner = CSF_AT(void*, self, 0x4);
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, owner, 0)[0xA4 / 4])(
                owner, nullptr, reinterpret_cast<float*>(positionBytes));

        CSF_AT(float, angleBytes, 0x4) = static_cast<float>(
            C_008342e8_ComputeResolvedObjectScalar(
                CSF_AT(C_008342e8*, self, 0x4), nullptr) * g_DegreesToRadians);

        uint8_t* directionBytes = requestBytes + 0x14;
        CSF_AT(float, directionBytes, 0x0) =
            CSF_AT(float, self, 0x14) - CSF_AT(float, positionBytes, 0x0);
        CSF_AT(float, directionBytes, 0x4) =
            CSF_AT(float, self, 0x18) - CSF_AT(float, positionBytes, 0x4);
        CSF_AT(float, directionBytes, 0x8) =
            CSF_AT(float, self, 0x1C) - CSF_AT(float, positionBytes, 0x8);

        const float targetAngle = static_cast<float>(
            S_00526d40_GetVectorAngleRadians(
                reinterpret_cast<S_00526d40*>(directionBytes), nullptr));
        CSF_AT(float, angleBytes, 0x8) = static_cast<float>(
            AngleDiffNormalized(targetAngle, CSF_AT(float, angleBytes, 0x4)));

        float turnLimit = 0.17453292f;
        if (CSF_AT(int16_t, self, 0x22) > 0)
        {
            void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            turnLimit = CSF_AT(float, settings, 0xAC0) * g_DegreesToRadians;
        }

        void* turnController =
            CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x30);
        const bool canTurn =
            ((bool(__fastcall*)(void*, void*))
                CSF_AT(void**, turnController, 0)[0xB8 / 4])(
                    turnController, nullptr);

        if (canTurn)
        {
            if (std::fabs(CSF_AT(float, angleBytes, 0x8)) > turnLimit)
            {
                bool shouldStart = CSF_AT(int8_t, self, 0x38) > 0;
                if (!shouldStart)
                {
                    void* actionController =
                        CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x48);
                    shouldStart =
                        ((int(__fastcall*)(void*, void*))
                            CSF_AT(void**, actionController, 0)[0x40 / 4])(
                                actionController, nullptr) == 0;
                }

                if (shouldStart)
                {
                    CSF_AT(float, correctionBytes, 0x0) = 0.0f;
                    if (CSF_AT(int16_t, self, 0x22) > 0)
                    {
                        CSF_AT(float, angleBytes, 0x0) =
                            turnLimit * g_CharacterHeightScale;
                        CSF_AT(float, correctionBytes, 0x0) =
                            CSF_AT(float, angleBytes, 0x8);
                        turnLimit = turnLimit * g_PreviewRightScale;
                        ClampFloatToBounds(
                            &CSF_AT(float, correctionBytes, 0x0),
                            &turnLimit,
                            &CSF_AT(float, angleBytes, 0x0));
                    }

                    char delayedTurn = 0;
                    if (CSF_AT(int8_t, self, 0x38) <= 0)
                        delayedTurn =
                            CSF_AT(float, self, 0x3C) >= g_fIntroVideoDuration;

                    const float requestedAngle =
                        CSF_AT(float, correctionBytes, 0x0) +
                        CSF_AT(float, angleBytes, 0x8) +
                        CSF_AT(float, angleBytes, 0x4);
                    C_0082f9f8_Start(
                        CSF_AT(C_0082f9f8*, CSF_AT(void*, self, 0x4), 0x30),
                        nullptr, requestedAngle, delayedTurn);
                }
            }
        }
        else
        {
            CSF_AT(float, self, 0x3C) = 0.0f;
        }
    }

    const bool firstEnabled = CSF_AT(int16_t, self, 0x22) > 0;
    CSF_AT(uint32_t, requestBytes, 0x8) = CSF_AT(uint32_t, self, 0x14);
    CSF_AT(uint32_t, requestBytes, 0xC) = CSF_AT(uint32_t, self, 0x18);
    CSF_AT(uint32_t, requestBytes, 0x10) = CSF_AT(uint32_t, self, 0x1C);
    CSF_AT(int, requestBytes, 0x4) = 0;
    CSF_AT(uint8_t, requestBytes, 0x0) = firstEnabled;
    ApplyTargetPointRequest(
        CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x4C),
        nullptr, requestBytes);

    const bool secondEnabled = CSF_AT(int16_t, self, 0x24) > 0;
    CSF_AT(int, requestBytes, 0x4) = 1;
    CSF_AT(uint8_t, requestBytes, 0x0) = secondEnabled;
    ApplyTargetPointRequest(
        CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x4C),
        nullptr, requestBytes);
}

HOOK(0x0048CD50, UpdateTargetFacingAndPointRequests, "auto");
