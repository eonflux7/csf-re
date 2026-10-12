#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (S_004aa310* self, void* , int a2));
CSF_FUNC(char, __fastcall, TEstatico_CopyTargetPositionIfFree, 0x004AF540, (S_004af450* self, void* , float* a2, uint32_t* a3));
CSF_FUNC(int, __stdcall, ConfigureLinkedObject, 0x004B2030, (int arg0, uint32_t* a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Scene_CanTraverseSegment, 0x004DF2E0, (void* self, void* , unsigned int segmentAddress));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t __fastcall AiCombat_AdvanceCandidatePosition(
    void* self, void* /*edx*/, C_0085e760* entry, int /*unused*/)
{
    alignas(8) uint8_t orientationBytes[0xC];
    alignas(8) uint8_t segmentBytes[0x18];
    alignas(8) uint8_t targetBytes[0x2C];

    float* segment = reinterpret_cast<float*>(segmentBytes);
    float* target = reinterpret_cast<float*>(targetBytes);

    CSF_AT(void*, targetBytes, 0x28) = self;
    CSF_AT(uint32_t, targetBytes, 0x24) = CSF_AT(uint32_t, self, 0xC);

    int linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
    void* position = GetObjectPositionVector(
        CSF_AT(void*, linkedObject, 0x2C), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
    void* actor = CSF_AT(void*, linkedObject, 0x2C);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, actor, 0)[0xA4 / 4])(
            actor, nullptr, orientationBytes);

    target[7] = CSF_AT(float, position, 0x4) + g_fReturnedConstant_0082E030;
    void* candidate = reinterpret_cast<void*>(
        CSF_AT(uint32_t, self, 0x4) +
        28 * CSF_AT(uint32_t, self, 0x14));
    target[6] = CSF_AT(float, position, 0);
    target[8] = CSF_AT(float, position, 0x8);

    bool valid = false;
    uint32_t result = 0;

    do
    {
        if (CSF_AT(float, candidate, 0x18) < g_fZero)
            break;

        linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
        linkedObject = linkedValue
            ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
        actor = CSF_AT(void*, linkedObject, 0x2C);

        void* geometry = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, actor, 0)[0x28 / 4])(actor, nullptr);
        void* bounds = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, geometry, 0)[0x28 / 4])(geometry, nullptr);

        const float distance =
            CSF_AT(float, bounds, 0xC) * g_HalfDurationScale +
            CSF_AT(float, candidate, 0x18);
        const float displacementX = distance * CSF_AT(float, candidate, 0xC);
        target[1] = distance * CSF_AT(float, candidate, 0x10);
        target[2] = distance * CSF_AT(float, candidate, 0x14);
        target[3] = displacementX + target[6];
        target[4] = target[1] + target[7];
        target[5] = target[2] + target[8];

        if (CSF_AT(float, candidate, 0x18) <= g_fZero)
            break;

        segment[0] = CSF_AT(float, candidate, 0);
        segment[1] = CSF_AT(float, candidate, 0x4);
        segment[2] = CSF_AT(float, candidate, 0x8);
        segment[3] = target[3];
        segment[4] = target[4];
        segment[5] = target[5];

        GetLinkedObjectValue_4DB5C0(entry, nullptr);
        const uint32_t sceneControl = CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
        const int traversalContext = GetCurrentItemCount(
            reinterpret_cast<void*>(sceneControl), nullptr);
        if (static_cast<uint8_t>(Scene_CanTraverseSegment(
                reinterpret_cast<void*>(traversalContext), nullptr,
                reinterpret_cast<unsigned int>(segment))) == 0)
            break;

        target[0] = 0.0f;
        target[1] = 0.0f;
        target[2] = 0.0f;
        linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
        auto* staticActor = linkedValue
            ? reinterpret_cast<S_004af450*>(linkedValue - 4) : nullptr;
        if (TEstatico_CopyTargetPositionIfFree(
                staticActor, nullptr, segment,
                reinterpret_cast<uint32_t*>(target)))
            break;

        const float targetDistance = CSF_AT(float, candidate, 0x18);
        const float targetDisplacementX =
            targetDistance * CSF_AT(float, candidate, 0xC);
        segment[4] = targetDistance * CSF_AT(float, candidate, 0x10);
        segment[5] = targetDistance * CSF_AT(float, candidate, 0x14);
        target[0] = targetDisplacementX + target[6];
        target[1] = segment[4] + target[7];
        target[2] = segment[5] + target[8];

        linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
        auto* aiActor = linkedValue
            ? reinterpret_cast<S_004aa310*>(linkedValue - 4) : nullptr;
        valid = static_cast<uint8_t>(AiCore_IsTargetPositionValid(
            aiActor, nullptr, reinterpret_cast<int>(target))) != 0;

        if (valid)
        {
            const int linkedState = GetLinkedObjectField10(entry, nullptr);
            AiCoverAttackTasks_ResetLinkedTaskState(
                entry, nullptr, linkedState + 32);

            const int lookup = ResolveEntryLookupValue(entry, nullptr);
            void* provider = GetCachedObjectProvider(
                reinterpret_cast<void*>(lookup), nullptr);
            GetLinkedObjectField10(entry, nullptr);
            ConfigureLinkedObject(
                reinterpret_cast<int>(provider),
                reinterpret_cast<uint32_t*>(target));
        }

        const float nextDistance =
            CSF_AT(float, candidate, 0x18) + g_MaxPercent;
        result = CSF_AT(uint32_t, targetBytes, 0x10);
        CSF_AT(float, candidate, 0) = target[3];
        CSF_AT(float, candidate, 0x18) = nextDistance;
        CSF_AT(uint32_t, candidate, 0x4) = result;
        CSF_AT(float, candidate, 0x8) = target[5];
    } while (false);

    if (!valid)
    {
        self = CSF_AT(void*, targetBytes, 0x28);
        result = CSF_AT(uint32_t, targetBytes, 0x24);
        const uint32_t nextIndex = CSF_AT(uint32_t, self, 0x14) + 1;
        CSF_AT(uint32_t, self, 0x14) = nextIndex;
        if (nextIndex >= result)
            return Dispatch_NoLinkedTarget(entry, nullptr, 1);
    }

    return result;
}

HOOK(0x004B8720, AiCombat_AdvanceCandidatePosition, "auto");
