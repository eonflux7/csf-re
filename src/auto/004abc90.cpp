#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiMoveTasks_CheckAssignedPosition, 0x004AA3D0, (void* self, void* edx, float* a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

static bool __fastcall S_004abb90_CheckAssignedCandidate(
    S_004abb90* self, void* /*edx*/, int candidate)
{
    int linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(candidate), nullptr);
    int objectAddress = linked ? linked - 4 : 0;
    if (CSF_AT(int, reinterpret_cast<void*>(objectAddress), 0x6D8) < 0)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(candidate), nullptr);
    objectAddress = linked ? linked - 4 : 0;
    void* assignedObject = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, objectAddress + 0x6D0));
    if (!assignedObject)
        return true;

    linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(candidate), nullptr);
    int handleOwnerAddress = linked ? linked - 4 : 0;

    linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(candidate), nullptr);
    void* candidateObject = reinterpret_cast<void*>(linked ? linked - 4 : 0);

    void* positionMember = reinterpret_cast<uint8_t*>(self) + 0x38;
    float* position = reinterpret_cast<float*>(
        AiCoverAttackTasks_GetPositionData(positionMember, nullptr));

    bool result = static_cast<uint8_t>(
        AiMoveTasks_CheckAssignedPosition(
            candidateObject, nullptr, position, handleOwnerAddress + 0x6D0)) == 0;

    if (result)
    {
        linked = GetLinkedObjectValue_4DB5C0(
            reinterpret_cast<void*>(candidate), nullptr);
        objectAddress = linked ? linked - 4 : 0;
        void* movingObject = CSF_AT(
            void*, reinterpret_cast<void*>(objectAddress), 0x2C);

        position = reinterpret_cast<float*>(
            AiCoverAttackTasks_GetPositionData(positionMember, nullptr));
        float* movingPosition = GetObjectPositionVector(movingObject, nullptr);
        float movingDx = movingPosition[0] - position[0];
        float movingDy = movingPosition[1] - position[1];
        float movingDz = movingPosition[2] - position[2];

        float* assignedPosition = GetObjectPositionVector(assignedObject, nullptr);
        position = reinterpret_cast<float*>(
            AiCoverAttackTasks_GetPositionData(positionMember, nullptr));
        float assignedDx = position[0] - assignedPosition[0];
        float assignedDy = position[1] - assignedPosition[1];
        float assignedDz = position[2] - assignedPosition[2];

        float movingDistanceSquared =
            movingDx * movingDx + movingDy * movingDy + movingDz * movingDz;
        float assignedDistanceSquared =
            assignedDx * assignedDx + assignedDy * assignedDy + assignedDz * assignedDz;
        result = assignedDistanceSquared > movingDistanceSquared;
    }

    g_AiSearchCostThisFrame += 5;
    return result;
}

HOOK(0x004ABC90, S_004abb90_CheckAssignedCandidate, "auto");
