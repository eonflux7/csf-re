#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(char, __fastcall, CGuiControl_FindNearbyChildPosition, 0x00450210, (CGuiControl* self, void* , int referenceObject, float maximumDistance, uint32_t* excludedFirst, uint32_t* excludedSecond, uint32_t* outputPosition));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char, __fastcall, AiFncStatic_CheckPositionPair, 0x004DC660, (void* self, void* edx, int a2, int a3));
CSF_FUNC(bool, __fastcall, AiFncStatic_CheckSegment, 0x004DF2E0, (void* self, void* edx, unsigned int arg0));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall S_004aa310_HasClearPathToPosition(
    S_004aa310* self, void* /*edx*/, float* targetPosition)
{
    if (static_cast<uint8_t>(AiCore_IsTargetPositionValid(
            self, nullptr, reinterpret_cast<int>(targetPosition))) == 0)
        return false;

    auto* linkedObject = reinterpret_cast<uint8_t*>(self) + 8;
    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    auto* owner = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue)) - 4
        : nullptr;
    void* actor = CSF_AT(void*, owner, 0x2C);

    auto* manager = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetValue8B0554()));
    auto* positionChecker = reinterpret_cast<uint8_t*>(
        CSF_AT(void*, manager, 0x1FD8)) + 0x54;

    if (!AiFncStatic_CheckPositionPair(
            positionChecker, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(actor) + 0x1C),
            reinterpret_cast<int>(targetPosition)))
        return false;

    alignas(8) uint8_t contextBytes[0x18];
    alignas(8) uint8_t segmentBytes[0x18];
    alignas(8) uint8_t outputBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];
    auto* segment = reinterpret_cast<float*>(segmentBytes);
    auto* output = reinterpret_cast<float*>(outputBytes);
    auto* direction = reinterpret_cast<float*>(directionBytes);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue)) - 4
        : nullptr;

    output[0] = targetPosition[0];
    output[1] = targetPosition[1] + g_fReturnedConstant_0082E030;
    output[2] = targetPosition[2];

    float* actorPosition = GetObjectPositionVector(
        CSF_AT(void*, owner, 0x2C), nullptr);
    direction[0] = actorPosition[0];
    direction[1] = actorPosition[1] + g_fReturnedConstant_0082E030;
    direction[2] = actorPosition[2];

    segment[0] = direction[0];
    segment[1] = direction[1];
    segment[2] = direction[2];
    segment[3] = output[0];
    segment[4] = output[1];
    segment[5] = output[2];

    uint32_t sceneField = CGuiControl_GetField012C(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
    int queryService = GetCurrentItemCount(
        reinterpret_cast<void*>(static_cast<uintptr_t>(sceneField)), nullptr);
    if (!AiFncStatic_CheckSegment(
            reinterpret_cast<void*>(static_cast<uintptr_t>(queryService)),
            nullptr, reinterpret_cast<unsigned int>(segmentBytes)))
        return false;

    direction[0] = segment[3] - segment[0];
    direction[1] = segment[4] - segment[1];
    direction[2] = segment[5] - segment[2];
    float distance = std::sqrt(
        direction[2] * direction[2] +
        direction[1] * direction[1] +
        direction[0] * direction[0]);

    SpatialQuery_BuildContext(
        reinterpret_cast<C_00833b88*>(contextBytes), nullptr,
        reinterpret_cast<uint32_t*>(segmentBytes),
        reinterpret_cast<uint32_t*>(directionBytes));

    output[0] = 0.0f;
    output[1] = 0.0f;
    output[2] = 0.0f;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue)) - 4
        : nullptr;
    auto* excludedActor = CSF_AT(uint32_t*, owner, 0x2C);

    return CGuiControl_FindNearbyChildPosition(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(contextBytes), distance,
        excludedActor, nullptr,
        reinterpret_cast<uint32_t*>(outputBytes)) == 0;
}

HOOK(0x004AF5C0, S_004aa310_HasClearPathToPosition, "auto");
