#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiAlert_TestCoverTarget, 0x004B2EA0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiDistracted_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

static void* AiAlert_GetLinkedBase(void* task)
{
    const int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return reinterpret_cast<void*>(
        linked ? static_cast<uint32_t>(linked) - 4u : 0u);
}

static void* AiAlert_MemberAddress(void* object, uint32_t offset)
{
    return reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(object) + offset);
}

static int AiAlert_FloatBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static void AiAlert_ClearTarget(void* linked, uint8_t* handleBytes)
{
    char* targetName = AiCore_GetTaskTargetNameBuffer(
        CSF_AT(S_0047ae10*, linked, 0x718), nullptr);
    ResetState(targetName, nullptr);

    void* character = CSF_AT(void*, linked, 0x2C);
    CSF_AT(void**, handleBytes, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(int, handleBytes, 4) = -1;
    CSF_AT(int, handleBytes, 8) = -1;
    AiAttackTasks_CopyTargetHandle(
        CSF_AT(void*, character, 0x204), nullptr,
        reinterpret_cast<int>(handleBytes));
}

static void __stdcall AiAlert_UpdateCoverAttackState(int a1, float delta)
{
    alignas(8) uint8_t targetHandleBytes[0xC];
    void* task = reinterpret_cast<void*>(a1);

    AiCoverAttackTasks_UpdateCharacterTasks(a1, AiAlert_FloatBits(delta));
    if (static_cast<uint8_t>(IsField10Null(task, nullptr)) != 0)
        return;

    void* linked = AiAlert_GetLinkedBase(task);
    bool updateCover = false;
    if (CSF_AT(int, linked, 0x608) == 7)
    {
        void* targetOwner = AiAlert_GetLinkedBase(task);
        void* testObject = AiAlert_GetLinkedBase(task);
        void* target = AiAlert_MemberAddress(
            targetOwner,
            CSF_AT(int, targetOwner, 0x630) < 0 ? 0x610 : 0x628);
        updateCover = AiAlert_TestCoverTarget(
            testObject, nullptr, reinterpret_cast<int>(target)) != 0;
    }

    if (updateCover)
    {
        GetLinkedObjectValue_4DB5C0(task, nullptr);
        linked = AiAlert_GetLinkedBase(task);
        AiCoverAttackTasks_UpdateCoverState(
            static_cast<float*>(AiAlert_MemberAddress(linked, 0x690)),
            nullptr, delta);
    }
    else
    {
        linked = AiAlert_GetLinkedBase(task);
        bool clearTarget =
            CSF_AT(int, linked, 0x608) == -1 &&
            CSF_AT(int, linked, 0x60C) == -1;

        if (!clearTarget)
        {
            linked = AiAlert_GetLinkedBase(task);
            clearTarget = CSF_AT(float, linked, 0x678) > g_TransitionOne;
            if (!clearTarget)
            {
                void* elapsedOwner = AiAlert_GetLinkedBase(task);
                void* durationOwner = AiAlert_GetLinkedBase(task);
                // The original also takes this branch for unordered comparisons.
                clearTarget = !(
                    CSF_AT(float, durationOwner, 0x698) * g_HalfDurationScale >
                    CSF_AT(float, elapsedOwner, 0x694));
            }
        }

        if (clearTarget)
        {
            linked = AiAlert_GetLinkedBase(task);
            AiAlert_ClearTarget(linked, targetHandleBytes);
            linked = AiAlert_GetLinkedBase(task);
            AiCoverAttackTasks_UpdateCoverState(
                static_cast<float*>(AiAlert_MemberAddress(linked, 0x690)),
                nullptr, -delta);
        }
        else
        {
            GetLinkedObjectValue_4DB5C0(task, nullptr);
            linked = AiAlert_GetLinkedBase(task);
            void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            void* durationState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            float duration = CSF_AT(float, durationState, 0xA94);
            int transition = CSF_AT(int, globalState, 0xA94);
            float halfDuration = duration * g_HalfDurationScale;
            AiCoverAttackTasks_SetCoverTransition(
                AiAlert_MemberAddress(linked, 0x690), nullptr,
                AiAlert_FloatBits(halfDuration), transition);
        }
    }

    linked = AiAlert_GetLinkedBase(task);
    bool resetLinkedTask = CSF_AT(uint8_t, linked, 0x684) != 0;
    if (!resetLinkedTask)
    {
        linked = AiAlert_GetLinkedBase(task);
        if (CSF_AT(int, linked, 0x608) != -1 ||
            CSF_AT(int, linked, 0x60C) != -1)
        {
            linked = AiAlert_GetLinkedBase(task);
            resetLinkedTask = (CSF_AT(uint8_t, linked, 0x680) & 1) == 0;
        }
    }

    if (resetLinkedTask)
    {
        GetLinkedObjectValue_4DB5C0(task, nullptr);
        int linkedField = GetLinkedObjectField10(task, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedField + 4);
    }

    linked = AiAlert_GetLinkedBase(task);
    AiDistracted_UpdateLinkedState(linked, nullptr);

    void* elapsedOwner = AiAlert_GetLinkedBase(task);
    void* durationOwner = AiAlert_GetLinkedBase(task);
    if (CSF_AT(float, durationOwner, 0x698) * g_ValueScale >
        CSF_AT(float, elapsedOwner, 0x694))
    {
        linked = AiAlert_GetLinkedBase(task);
        AiAlert_ClearTarget(linked, targetHandleBytes);
        int linkedField = GetLinkedObjectField10(task, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedField + 16);
    }
}

HOOK(0x004B4460, AiAlert_UpdateCoverAttackState, "auto");
