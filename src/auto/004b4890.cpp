#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, StaticAi_SetGuardStateFromArgs, 0x004B0100, (C_0085e7b8* a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int AiAlert_FloatBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static uint8_t* AiAlert_LinkedObjectBase(int linkedValue)
{
    return linkedValue
        ? reinterpret_cast<uint8_t*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;
}

static uint32_t* __stdcall AiAlert_UpdateCoverAttack(
    C_0085e760* self, float deltaTime)
{
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(self), AiAlert_FloatBits(deltaTime));

    const int field10Null = IsField10Null(self, nullptr);
    if (static_cast<uint8_t>(field10Null))
        return reinterpret_cast<uint32_t*>(field10Null);

    uint8_t* linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));

    bool resetCover =
        CSF_AT(int, linked, 0x608) == -1 &&
        CSF_AT(int, linked, 0x60C) == -1;

    if (!resetCover)
    {
        linked = AiAlert_LinkedObjectBase(
            GetLinkedObjectValue_4DB5C0(self, nullptr));
        resetCover = CSF_AT(float, linked, 0x678) > g_TransitionOne;
    }

    if (resetCover)
    {
        linked = AiAlert_LinkedObjectBase(
            GetLinkedObjectValue_4DB5C0(self, nullptr));

        // These two call addresses are also classified as vtables in the packet.
        (void)CSF_VTBL(0x0047AE10);
        char* targetName = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, linked, 0x718), nullptr);
        ResetState(targetName, nullptr);

        void* character = CSF_AT(void*, linked, 0x2C);
        alignas(8) uint8_t targetHandleBytes[0xC];
        CSF_AT(void*, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, targetHandleBytes, 0x4) = -1;
        CSF_AT(int, targetHandleBytes, 0x8) = -1;

        (void)CSF_VTBL(0x0048C9F0);
        AiAttackTasks_CopyTargetHandle(
            CSF_AT(void*, character, 0x204), nullptr,
            reinterpret_cast<int>(targetHandleBytes));

        linked = AiAlert_LinkedObjectBase(
            GetLinkedObjectValue_4DB5C0(self, nullptr));
        AiCoverAttackTasks_UpdateCoverState(
            reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(linked) + 0x690),
            nullptr, -deltaTime);
    }
    else
    {
        GetLinkedObjectValue_4DB5C0(self, nullptr);
        linked = AiAlert_LinkedObjectBase(
            GetLinkedObjectValue_4DB5C0(self, nullptr));

        void* firstGlobal = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        void* secondGlobal = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        const float duration = CSF_AT(float, secondGlobal, 0xA94);
        const int fullDurationBits = CSF_AT(int, firstGlobal, 0xA94);
        const float halfDuration = duration * g_HalfDurationScale;

        AiCoverAttackTasks_SetCoverTransition(
            reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(linked) + 0x690),
            nullptr, AiAlert_FloatBits(halfDuration), fullDurationBits);
    }

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    if (CSF_AT(int, linked, 0x608) == -1 &&
        CSF_AT(int, linked, 0x60C) == -1)
        return reinterpret_cast<uint32_t*>(linked);

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    if (!CSF_AT(int, linked, 0x2D8))
        return reinterpret_cast<uint32_t*>(linked);

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    void* moveTask = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(linked) + 0x2D4);
    reinterpret_cast<void (__fastcall*)(void*, void*, C_0085e760*, float)>(
        CSF_AT(void**, moveTask, 0)[0x2C / 4])(
            moveTask, nullptr, self, deltaTime);

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    const int moveState = CSF_AT(int, linked, 0x2D8);
    if (moveState != 2 && moveState != 3)
        return reinterpret_cast<uint32_t*>(moveState);

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    void* character = CSF_AT(void*, linked, 0x2C);
    void* controller = CSF_AT(void*, character, 0x44);
    const bool ready = reinterpret_cast<bool (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x90 / 4])(controller, nullptr);
    if (!ready)
        return nullptr;

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    if (CSF_AT(int, linked, 0x2D8) == 3)
    {
        const int linkedTask = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(
            self, nullptr, linkedTask + 12);

        linked = AiAlert_LinkedObjectBase(
            GetLinkedObjectValue_4DB5C0(self, nullptr));
        const int guardArgs = static_cast<int>(
            reinterpret_cast<uintptr_t>(linked) + 0x2E8);

        void* entry = reinterpret_cast<void*>(
            ResolveEntryLookupValue(self, nullptr));
        C_0085e7b8* provider = reinterpret_cast<C_0085e7b8*>(
            GetCachedObjectProvider(entry, nullptr));
        GetLinkedObjectField10(self, nullptr);
        return reinterpret_cast<uint32_t*>(
            StaticAi_SetGuardStateFromArgs(provider, guardArgs));
    }

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    moveTask = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(linked) + 0x2D4);
    reinterpret_cast<void (__fastcall*)(void*, void*, C_0085e760*)>(
        CSF_AT(void**, moveTask, 0)[0x28 / 4])(moveTask, nullptr, self);

    linked = AiAlert_LinkedObjectBase(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    CSF_AT(float, linked, 0x2E0) = 1.0f;
    CSF_AT(float, linked, 0x2E4) = 1.0f;
    return reinterpret_cast<uint32_t*>(linked);
}

HOOK(0x004B4890, AiAlert_UpdateCoverAttack, "auto");
