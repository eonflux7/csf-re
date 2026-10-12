#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_004aa310_HasClearPathToPosition, 0x004AF5C0, (S_004aa310* self, void* , float* targetPosition));
CSF_FUNC(int, __fastcall, AiStateFactory_UpdateTargetPosition, 0x004B1310, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __stdcall AiStateFactory_HandleMovementRequest(int request)
{
    void* requestObject = reinterpret_cast<void*>(request);

    auto* initialTarget = reinterpret_cast<uint8_t*>(
        GetLinkedObjectField10(requestObject, nullptr)) + 160;
    auto* positionState = reinterpret_cast<uint8_t*>(
        GetLinkedObjectField10(requestObject, nullptr)) + 0x64;
    AiStateFactory_UpdateTargetPosition(
        positionState, nullptr, reinterpret_cast<uint32_t*>(initialTarget));

    int linkedValue = GetLinkedObjectValue_4DB5C0(requestObject, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    C_008342e8* actor = CSF_AT(C_008342e8*, linkedObject, 0x2C);

    auto* targetPosition = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(requestObject, nullptr)) + 160);
    float* actorPosition = GetObjectPositionVector(actor, nullptr);
    float deltaX = actorPosition[0] - targetPosition[0];
    float deltaZ = actorPosition[2] - targetPosition[2];
    if (!(deltaX * deltaX + deltaZ * deltaZ > g_TransitionOne))
        return 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(requestObject, nullptr);
    auto* pathState = reinterpret_cast<S_004aa310*>(linkedValue ? linkedValue - 4 : 0);
    targetPosition = reinterpret_cast<float*>(
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(requestObject, nullptr)) + 160);
    if (!S_004aa310_HasClearPathToPosition(pathState, nullptr, targetPosition))
        return 0;

    GetLinkedObjectField10(requestObject, nullptr);
    GetLinkedObjectField10(requestObject, nullptr);
    GetLinkedObjectField10(requestObject, nullptr);
    GetLinkedObjectField10(requestObject, nullptr);
    int taskState = GetLinkedObjectField10(requestObject, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(requestObject, nullptr, taskState + 8);
    return 1;
}

HOOK(0x004B72D0, AiStateFactory_HandleMovementRequest, "auto");
