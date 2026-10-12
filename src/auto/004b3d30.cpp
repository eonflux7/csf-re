#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, TEstatico_UpdateMissionActiveFlag, 0x004AF360, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void* AiAlert_ResolveLinkedObject(int entry)
{
    const int linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(entry), nullptr);
    return linked
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linked) - 4)
        : nullptr;
}

static void __stdcall AiAlert_UpdateResolvedDistractedEntry(
    int entry, float delta, float decay)
{
    void* object = AiAlert_ResolveLinkedObject(entry);
    bool missionActive = false;
    if (CSF_AT(uint8_t, object, 0x684))
    {
        object = AiAlert_ResolveLinkedObject(entry);
        missionActive = CSF_AT(int, object, 0x608) == 7;
    }

    if (missionActive)
    {
        object = AiAlert_ResolveLinkedObject(entry);
        TEstatico_UpdateMissionActiveFlag(object, nullptr);

        object = AiAlert_ResolveLinkedObject(entry);
        AiCoverAttackTasks_UpdateCoverState(
            reinterpret_cast<float*>(static_cast<uint8_t*>(object) + 0x690),
            nullptr, delta);

        GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(entry), nullptr);
        return;
    }

    object = AiAlert_ResolveLinkedObject(entry);
    bool shouldDecay =
        CSF_AT(int, object, 0x608) == -1 &&
        CSF_AT(int, object, 0x60C) == -1;

    if (!shouldDecay)
    {
        object = AiAlert_ResolveLinkedObject(entry);
        shouldDecay = CSF_AT(float, object, 0x678) > g_TransitionOne;
        if (!shouldDecay)
        {
            void* elapsedObject = AiAlert_ResolveLinkedObject(entry);
            void* durationObject = AiAlert_ResolveLinkedObject(entry);
            shouldDecay = !(
                CSF_AT(float, durationObject, 0x698) * g_HalfDurationScale >
                CSF_AT(float, elapsedObject, 0x694));
        }
    }

    if (!shouldDecay)
    {
        object = AiAlert_ResolveLinkedObject(entry);
        void* firstGlobal = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        void* secondGlobal = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

        const float duration = CSF_AT(float, secondGlobal, 0xA94);
        const int fullDurationBits = CSF_AT(int, firstGlobal, 0xA94);
        const float halfDuration = duration * g_HalfDurationScale;
        int halfDurationBits;
        std::memcpy(&halfDurationBits, &halfDuration, sizeof(halfDurationBits));

        AiCoverAttackTasks_SetCoverTransition(
            static_cast<uint8_t*>(object) + 0x690, nullptr,
            halfDurationBits, fullDurationBits);
        GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(entry), nullptr);
        return;
    }

    if (!(decay > g_fZero))
        return;

    object = AiAlert_ResolveLinkedObject(entry);
    AiCoverAttackTasks_UpdateCoverState(
        reinterpret_cast<float*>(static_cast<uint8_t*>(object) + 0x690),
        nullptr, -decay);

    object = AiAlert_ResolveLinkedObject(entry);
    bool clearTarget =
        CSF_AT(int, object, 0x608) == -1 &&
        CSF_AT(int, object, 0x60C) == -1;
    if (!clearTarget)
    {
        object = AiAlert_ResolveLinkedObject(entry);
        clearTarget = CSF_AT(float, object, 0x678) > g_TransitionOne;
    }

    if (clearTarget)
    {
        object = AiAlert_ResolveLinkedObject(entry);
        char* targetName = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, object, 0x718), nullptr);
        ResetState(targetName, nullptr);

        void* owner = CSF_AT(void*, object, 0x2C);
        alignas(8) uint8_t targetHandleBytes[0xC];
        CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, targetHandleBytes, 0x4) = -1;
        CSF_AT(int, targetHandleBytes, 0x8) = -1;

        AiAttackTasks_CopyTargetHandle(
            CSF_AT(void*, owner, 0x204), nullptr,
            reinterpret_cast<int>(targetHandleBytes));
    }
}

HOOK(0x004B3D30, AiAlert_UpdateResolvedDistractedEntry, "auto");
