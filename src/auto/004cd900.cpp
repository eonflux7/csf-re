#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(int, __fastcall, IsField90Null, 0x0040C910, (void* self, void*));
CSF_FUNC(bool, __fastcall, AiSearchOrders_AimControllerAtPosition, 0x0040CA40, (void* self, void* edx, float* arg0, char a4));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float angle));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(double, __cdecl, SearchSweep_ClampAngleToArc, 0x004CD490, (float angle, float center, float halfWidth));
CSF_FUNC(void, __fastcall, C_0083ab58_IssueTargetCheckRequest, 0x004CD870, (C_0083ab58* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_DefaultAngleLimit, 0x0082D7BC);
CSF_GLOBAL(float, g_SearchAngleScale, 0x0083AB30);
CSF_GLOBAL(float, g_SearchDegreesToRadians, 0x0083AB34);
CSF_GLOBAL(float, g_SearchTurnRate, 0x0083AC1C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static const auto g_SearchOwnerPointerAccessorVtable = CSF_VTBL(0x00492B20);

static void __fastcall C_0083ab58_UpdateSearchFacing(
    C_0083ab58* self, void* /*edx*/, float deltaTime)
{
    alignas(8) uint8_t vectorBytes[0x14];
    S_00526d40& direction = *reinterpret_cast<S_00526d40*>(vectorBytes);

    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                            reinterpret_cast<int>(&self->field_0044)));
    if (!target)
    {
        CSF_AT(uint32_t, self, 0x48) = -1;
        self->field_004c = -1;
        self->field_0040 = 3;
        return;
    }

    void* owner = reinterpret_cast<void*>(self->field_0014);
    if (static_cast<uint8_t>(IsField1BCNonNegative(owner, nullptr)))
    {
        void* scene = g_pCurrentScene;
        char* handle = S_00492b20_GetPtr0x1B4(owner, nullptr);
        void* resolved = reinterpret_cast<void*>(
            CHandleTable_Resolve(scene, nullptr, reinterpret_cast<int>(handle)));
        if (!resolved || CSF_AT(uint32_t, resolved, 0x50) != 19)
            return;

        void* controller = CSF_AT(void*, resolved, 0x4C);
        float* position =
            reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
                target->vtbl[0xA4 / 4])(
                    target, nullptr, reinterpret_cast<float*>(vectorBytes));
        AiSearchOrders_AimControllerAtPosition(controller, nullptr, position, 0);
        if (!static_cast<uint8_t>(IsField90Null(controller, nullptr)))
            return;
    }
    else
    {
        // These two scalar slots belong to the same escaped stack object.
        CSF_AT(float, vectorBytes, 0x10) = static_cast<float>(
            C_008342e8_ComputeResolvedObjectScalar(
                reinterpret_cast<C_008342e8*>(self->field_0014), nullptr)
            * g_SearchDegreesToRadians);

        float* ownerPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0014), nullptr);
        float* targetPosition = GetObjectPositionVector(target, nullptr);

        float dz = targetPosition[2] - ownerPosition[2];
        float dy = targetPosition[1] - ownerPosition[1];
        CSF_AT(float, vectorBytes, 0x00) = targetPosition[0] - ownerPosition[0];
        CSF_AT(float, vectorBytes, 0x04) = dy;
        CSF_AT(float, vectorBytes, 0x08) = dz;

        double angle = S_00526d40_GetVectorAngleRadians(&direction, nullptr);
        const float currentAngle = CSF_AT(float, vectorBytes, 0x10);
        CSF_AT(float, vectorBytes, 0x0C) = static_cast<float>(angle);
        const float targetAngle = CSF_AT(float, vectorBytes, 0x0C);
        const float turnStep = deltaTime * g_SearchTurnRate;

        double clampedAngle =
            SearchSweep_ClampAngleToArc(targetAngle, currentAngle, turnStep);
        DispatchSceneObjectWithPayload(
            reinterpret_cast<void*>(self->field_0014), nullptr,
            static_cast<float>(clampedAngle * g_SearchAngleScale));

        double difference = AngleDiffNormalized(targetAngle, currentAngle);
        if (!(std::fabs(difference) < g_DefaultAngleLimit))
            return;
    }

    auto* currentOwner = reinterpret_cast<C_008342e8*>(self->field_0014);
    void* linkedObject = reinterpret_cast<void*>(currentOwner->field_0044);
    if (linkedObject &&
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0x90 / 4])(linkedObject, nullptr))
    {
        C_0083ab58_IssueTargetCheckRequest(self, nullptr);
        self->field_0040 = 4;
    }
}

HOOK(0x004CD900, C_0083ab58_UpdateSearchFacing, "auto");
