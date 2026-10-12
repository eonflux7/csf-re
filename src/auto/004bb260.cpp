#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(int, __cdecl, AiTaskStates_IsDistanceWithin200, 0x0041E350, (S_004e0c20* a1, S_004e0c20* a2));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, ComputeLinkedSubObjectRatio, 0x004A9CD0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, IsGuardStateValidForTarget, 0x004AA310, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_IsMovementBlocked, 0x004AA5A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiDistracted_GrowPointList, 0x004ADDB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (S_004aa310* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* , unsigned int a2));
CSF_FUNC(void*, __fastcall, InitDefaultFieldBlock, 0x004DEF80, (void* self, void*));
CSF_FUNC(char, __fastcall, S_004dfbc0_TrySetupSubObjectPair, 0x004E07B0, (S_004dfbc0* self, void* , unsigned int a2, int a3));
CSF_FUNC(bool, __fastcall, C_0083e9d0_TestPairWithSelfVector, 0x004E5C30, (C_0083e9d0* self, void* , float* a2, float* a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_fQuarterPi, 0x0082FAC8);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static const auto g_GuardStateVtable = CSF_VTBL(0x004AA310);

static uint8_t* AiDistracted_GetOwner(void* task)
{
    const int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static uint8_t* AiDistracted_GetTaskData(void* task)
{
    return reinterpret_cast<uint8_t*>(GetLinkedObjectField10(task, nullptr));
}

static void AiDistracted_CopyPoint(void* destination, const void* source)
{
    CSF_AT(uint32_t, destination, 0x00) = CSF_AT(uint32_t, source, 0x00);
    CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, source, 0x08);
    CSF_AT(uint32_t, destination, 0x0C) = CSF_AT(uint32_t, source, 0x0C);
    CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, source, 0x10);
}

static float AiDistracted_DistanceSquared(const float* position, const float* reference)
{
    const float x = position[0] - reference[0];
    const float y = position[1] - reference[1];
    const float z = position[2] - reference[2];
    return x * x + z * z + y * y;
}

static uint32_t* __stdcall AiDistracted_ProcessNearestPoint(
    S_004aa310* task, int /*unused*/)
{
    uint8_t* ownerForPosition = AiDistracted_GetOwner(task);
    uint8_t* ownerForValidation = AiDistracted_GetOwner(task);
    uint32_t* ownerPosition = GetObjectPositionVector(
        CSF_AT(void*, ownerForPosition, 0x2C), nullptr);
    if (!static_cast<uint8_t>(AiCore_IsTargetPositionValid(
            reinterpret_cast<S_004aa310*>(ownerForValidation), nullptr,
            reinterpret_cast<int>(ownerPosition))))
    {
        return reinterpret_cast<uint32_t*>(
            Dispatch_NoLinkedTarget(task, nullptr, 0));
    }

    const uint32_t pointCount =
        CSF_AT(uint32_t, AiDistracted_GetTaskData(task), 0x18);
    if (pointCount == 0)
    {
        uint8_t* owner = AiDistracted_GetOwner(task);
        if (CSF_AT(uint32_t, CSF_AT(void*, owner, 0x2C), 0x1D8) == 3)
        {
            owner = AiDistracted_GetOwner(task);
            if (AiCoverAttackTasks_IsMovementBlocked(owner, nullptr))
            {
                uint8_t* data = AiDistracted_GetTaskData(task);
                return reinterpret_cast<uint32_t*>(
                    AiCoverAttackTasks_ResetLinkedTaskState(data + 12, nullptr, 0));
            }
        }
        return reinterpret_cast<uint32_t*>(
            Dispatch_NoLinkedTarget(task, nullptr, 0));
    }

    uint8_t* owner = AiDistracted_GetOwner(task);
    float* position = reinterpret_cast<float*>(GetObjectPositionVector(
        CSF_AT(void*, owner, 0x2C), nullptr));
    uint32_t nearestIndex = 0;

    void* firstPoint = CSF_AT(void*, AiDistracted_GetTaskData(task), 0x10);
    AiDistracted_CopyPoint(AiDistracted_GetTaskData(task) + 48, firstPoint);

    owner = AiDistracted_GetOwner(task);
    float* initialPosition = reinterpret_cast<float*>(GetObjectPositionVector(
        CSF_AT(void*, owner, 0x2C), nullptr));
    float* pointPosition = reinterpret_cast<float*>(AiCoverAttackTasks_GetPositionData(
        AiDistracted_GetTaskData(task) + 48, nullptr));
    float nearestDistance =
        AiDistracted_DistanceSquared(pointPosition, initialPosition);

    uint32_t pointOffset = 20;
    for (uint32_t index = 1; index < pointCount; ++index, pointOffset += 20)
    {
        uint8_t* data = AiDistracted_GetTaskData(task);
        pointPosition = reinterpret_cast<float*>(AiCoverAttackTasks_GetPositionData(
            CSF_AT(uint8_t*, data, 0x10) + pointOffset, nullptr));
        const float distance = AiDistracted_DistanceSquared(pointPosition, position);
        if (distance < nearestDistance)
        {
            nearestDistance = distance;
            nearestIndex = index;
        }
    }

    uint8_t* points = CSF_AT(uint8_t*, AiDistracted_GetTaskData(task), 0x10);
    const uint32_t nearestOffset = 20 * nearestIndex;
    void* nearestPoint = points + nearestOffset;
    AiDistracted_CopyPoint(AiDistracted_GetTaskData(task) + 48, nearestPoint);

    pointPosition = reinterpret_cast<float*>(AiCoverAttackTasks_GetPositionData(
        AiDistracted_GetTaskData(task) + 48, nullptr));
    const float horizontalX = pointPosition[0] - position[0];
    const float horizontalZ = pointPosition[2] - position[2];

    if (horizontalX * horizontalX + horizontalZ * horizontalZ > g_MaxPercent)
    {
        owner = AiDistracted_GetOwner(task);
        pointPosition = reinterpret_cast<float*>(AiCoverAttackTasks_GetPositionData(
            AiDistracted_GetTaskData(task) + 48, nullptr));
        const float firstDistance = AiDistracted_DistanceSquared(
            pointPosition, reinterpret_cast<float*>(owner + 0x640));

        owner = AiDistracted_GetOwner(task);
        pointPosition = reinterpret_cast<float*>(AiCoverAttackTasks_GetPositionData(
            AiDistracted_GetTaskData(task) + 48, nullptr));
        const float secondDistance = AiDistracted_DistanceSquared(
            pointPosition, reinterpret_cast<float*>(owner + 0x64C));

        owner = AiDistracted_GetOwner(task);
        float* activePosition = reinterpret_cast<float*>(
            owner + 1612 + 12 * (CSF_AT(uint32_t, owner, 0x638) != 0));
        pointPosition = reinterpret_cast<float*>(AiCoverAttackTasks_GetPositionData(
            AiDistracted_GetTaskData(task) + 48, nullptr));
        const float activeDistance =
            AiDistracted_DistanceSquared(pointPosition, activePosition);

        if (activeDistance <= firstDistance && activeDistance <= secondDistance)
        {
            alignas(8) uint8_t vectorBytes[0x1C];
            S_00526d40& vector = *reinterpret_cast<S_00526d40*>(vectorBytes);
            bool candidateAccepted = true;
            GetValue8B0554();
            const float candidateAngle = static_cast<float>(
                ComputeLinkedSubObjectRatio(AiDistracted_GetTaskData(task) + 48, nullptr));
            uint32_t existingCount =
                CSF_AT(uint32_t, AiDistracted_GetTaskData(task), 0x28);

            uint32_t existingOffset = 0;
            for (uint32_t index = 0; index < existingCount && candidateAccepted;
                 ++index, existingOffset += 20)
            {
                uint8_t* data = AiDistracted_GetTaskData(task);
                void* existingPoint = CSF_AT(uint8_t*, data, 0x20) + existingOffset;
                float* candidatePosition = reinterpret_cast<float*>(
                    AiCoverAttackTasks_GetPositionData(
                        AiDistracted_GetTaskData(task) + 48, nullptr));
                float* existingPosition = reinterpret_cast<float*>(
                    AiCoverAttackTasks_GetPositionData(existingPoint, nullptr));
                CSF_AT(float, vectorBytes, 0x00) =
                    existingPosition[0] - candidatePosition[0];
                CSF_AT(float, vectorBytes, 0x04) =
                    existingPosition[1] - candidatePosition[1];
                CSF_AT(float, vectorBytes, 0x08) =
                    existingPosition[2] - candidatePosition[2];
                const float angle = static_cast<float>(
                    S_00526d40_GetVectorAngleRadians(&vector, nullptr));
                const double difference = AngleDiffNormalized(candidateAngle, angle);
                candidateAccepted = std::fabs(difference) >= g_fQuarterPi;
            }

            if (candidateAccepted)
            {
                alignas(8) uint8_t testPositionBytes[0xC];
                float* testPosition = reinterpret_cast<float*>(testPositionBytes);
                testPosition[0] = 0.0f;
                testPosition[1] = 0.0f;
                CSF_AT(uint32_t, testPositionBytes, 0x08) = 0;

                uint32_t* candidatePosition = AiCoverAttackTasks_GetPositionData(
                    AiDistracted_GetTaskData(task) + 48, nullptr);
                CSF_AT(uint32_t, vectorBytes, 0x00) = candidatePosition[0];
                CSF_AT(uint32_t, vectorBytes, 0x04) = candidatePosition[1];
                CSF_AT(uint32_t, vectorBytes, 0x08) = candidatePosition[2];
                CSF_AT(float, vectorBytes, 0x04) += g_fReturnedConstant_0082E030;

                existingCount = CSF_AT(uint32_t, AiDistracted_GetTaskData(task), 0x28);
                existingOffset = 0;
                for (uint32_t index = 0; index < existingCount && candidateAccepted;
                     ++index, existingOffset += 20)
                {
                    uint8_t* data = AiDistracted_GetTaskData(task);
                    uint32_t* existingPosition = AiCoverAttackTasks_GetPositionData(
                        CSF_AT(uint8_t*, data, 0x20) + existingOffset, nullptr);
                    CSF_AT(uint32_t, testPositionBytes, 0x00) = existingPosition[0];
                    CSF_AT(uint32_t, testPositionBytes, 0x04) = existingPosition[1];
                    CSF_AT(uint32_t, testPositionBytes, 0x08) = existingPosition[2];
                    testPosition[1] += g_fReturnedConstant_0082E030;
                    owner = AiDistracted_GetOwner(task);
                    void* actor = CSF_AT(void*, owner, 0x2C);
                    candidateAccepted = !C_0083e9d0_TestPairWithSelfVector(
                        CSF_AT(C_0083e9d0*, actor, 0x3C), nullptr,
                        testPosition, reinterpret_cast<float*>(vectorBytes));
                }

                if (candidateAccepted)
                {
                    uint8_t* candidateOwner = AiDistracted_GetOwner(task);
                    S_004e0c20* candidate = reinterpret_cast<S_004e0c20*>(
                        AiCoverAttackTasks_GetPositionData(
                            AiDistracted_GetTaskData(task) + 48, nullptr));
                    uint32_t sceneObject = CGuiControl_GetField012C(
                        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
                    S_004e0c20* subObject = reinterpret_cast<S_004e0c20*>(
                        SceneFindSubObject(reinterpret_cast<void*>(sceneObject), nullptr,
                            reinterpret_cast<unsigned int>(candidate)));

                    if (subObject &&
                        !(CSF_AT(uint8_t, subObject, 0x04) & 1) &&
                        AiTaskStates_IsDistanceWithin200(subObject, candidate) &&
                        IsGuardStateValidForTarget(candidateOwner, nullptr,
                            reinterpret_cast<int>(candidate)))
                    {
                        alignas(8) uint8_t fieldBlockBytes[0x4C];
                        alignas(8) uint8_t queryBytes[0x34];
                        InitDefaultFieldBlock(fieldBlockBytes, nullptr);
                        CSF_AT(uint32_t, queryBytes, 0x00) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x04) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x08) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x0C) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x10) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x14) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x18) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x1C) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x20) = 0;
                        CSF_AT(int, queryBytes, 0x24) = 2139095039;
                        CSF_AT(uint32_t, queryBytes, 0x28) = 0;
                        CSF_AT(uint32_t, queryBytes, 0x2C) = 0;

                        owner = AiDistracted_GetOwner(task);
                        uint32_t* start = GetObjectPositionVector(
                            CSF_AT(void*, owner, 0x2C), nullptr);
                        CSF_AT(uint32_t, queryBytes, 0x00) = start[0];
                        CSF_AT(uint32_t, queryBytes, 0x04) = start[1];
                        CSF_AT(uint32_t, queryBytes, 0x08) = start[2];
                        uint32_t* end = AiCoverAttackTasks_GetPositionData(
                            AiDistracted_GetTaskData(task) + 48, nullptr);
                        CSF_AT(uint32_t, queryBytes, 0x0C) = end[0];
                        CSF_AT(uint32_t, queryBytes, 0x10) = end[1];
                        CSF_AT(uint32_t, queryBytes, 0x14) = end[2];
                        CSF_AT(uint32_t, queryBytes, 0x24) =
                            CSF_AT(uint32_t,
                                reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xAB0);
                        sceneObject = CGuiControl_GetField012C(
                            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
                        S_004dfbc0* pairManager = reinterpret_cast<S_004dfbc0*>(
                            GetCurrentItemCount(reinterpret_cast<void*>(sceneObject), nullptr));
                        const char setupSucceeded = S_004dfbc0_TrySetupSubObjectPair(
                            pairManager, nullptr,
                            reinterpret_cast<unsigned int>(queryBytes),
                            reinterpret_cast<int>(fieldBlockBytes));

                        if (CSF_AT(void*, fieldBlockBytes, 0x3C))
                        {
                            Mem_Free(CSF_AT(void*, fieldBlockBytes, 0x3C));
                            CSF_AT(uint32_t, fieldBlockBytes, 0x3C) = 0;
                        }
                        CSF_AT(uint32_t, fieldBlockBytes, 0x10) = 0;
                        CSF_AT(int, fieldBlockBytes, 0x18) = -1082130432;
                        if (CSF_AT(void*, fieldBlockBytes, 0x08))
                            Mem_Free(CSF_AT(void*, fieldBlockBytes, 0x08));

                        if (setupSucceeded)
                        {
                            uint8_t* source = AiDistracted_GetTaskData(task) + 48;
                            uint8_t* data = AiDistracted_GetTaskData(task);
                            uint8_t* list = data + 32;
                            if (CSF_AT(uint32_t, data, 0x28) >=
                                CSF_AT(uint32_t, data, 0x24))
                            {
                                AiDistracted_GrowPointList(
                                    list, nullptr, CSF_AT(int, data, 0x2C));
                            }
                            const uint32_t count = CSF_AT(uint32_t, list, 0x08);
                            uint8_t* storage = CSF_AT(uint8_t*, list, 0x00);
                            CSF_AT(uint32_t, list, 0x08) = count + 1;
                            AiDistracted_CopyPoint(storage + 20 * count, source);
                            data = AiDistracted_GetTaskData(task);
                            AiCoverAttackTasks_ResetLinkedTaskState(data + 8, nullptr, 0);
                        }
                    }
                }
            }
        }
    }

    uint8_t* data = AiDistracted_GetTaskData(task);
    const uint32_t remaining = CSF_AT(uint32_t, data, 0x18) - 1;
    uint32_t* result = reinterpret_cast<uint32_t*>(data + 16);
    CSF_AT(uint32_t, data, 0x18) = remaining;
    if (nearestIndex != remaining)
    {
        uint8_t* storage = CSF_AT(uint8_t*, result, 0x00);
        return reinterpret_cast<uint32_t*>(_memcpy(
            storage + nearestOffset, storage + nearestOffset + 20,
            20 * (remaining - nearestIndex)));
    }
    return result;
}

HOOK(0x004BB260, AiDistracted_ProcessNearestPoint, "auto");
