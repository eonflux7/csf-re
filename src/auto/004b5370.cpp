#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __fastcall, S_0048c9f0_CopyTargetHandle, 0x0048C9F0, (S_0048c9f0* self, void* , const void* handle));
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

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static uint8_t* C_0085e760_GetSearchCharacter(C_0085e760* self)
{
    const int linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    return linked
        ? reinterpret_cast<uint8_t*>(static_cast<uint32_t>(linked) - 4)
        : nullptr;
}

static uint32_t* __stdcall C_0085e760_UpdateSearch(
    C_0085e760* self, float deltaTime)
{
    int deltaTimeBits;
    std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(self), deltaTimeBits);

    const int missingLinkedObject = IsField10Null(self, nullptr);
    if (static_cast<uint8_t>(missingLinkedObject))
        return reinterpret_cast<uint32_t*>(missingLinkedObject);

    uint8_t* character = C_0085e760_GetSearchCharacter(self);
    bool refreshCover =
        CSF_AT(int, character, 0x608) == -1 &&
        CSF_AT(int, character, 0x60C) == -1;

    if (!refreshCover)
    {
        character = C_0085e760_GetSearchCharacter(self);
        refreshCover = CSF_AT(float, character, 0x678) > g_TransitionOne;
    }

    if (refreshCover)
    {
        character = C_0085e760_GetSearchCharacter(self);
        char* targetName = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, character, 0x718), nullptr);
        ResetState(targetName, nullptr);

        void* owner = CSF_AT(void*, character, 0x2C);
        alignas(8) uint8_t targetHandleBytes[0xC];
        CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, targetHandleBytes, 0x4) = -1;
        CSF_AT(int, targetHandleBytes, 0x8) = -1;
        S_0048c9f0_CopyTargetHandle(
            CSF_AT(S_0048c9f0*, owner, 0x204), nullptr, targetHandleBytes);

        character = C_0085e760_GetSearchCharacter(self);
        AiCoverAttackTasks_UpdateCoverState(
            reinterpret_cast<float*>(character + 0x690), nullptr, deltaTime);
    }
    else
    {
        GetLinkedObjectValue_4DB5C0(self, nullptr);
        character = C_0085e760_GetSearchCharacter(self);
        void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        AiCoverAttackTasks_SetCoverTransition(
            character + 0x690, nullptr, 0,
            CSF_AT(int, globalState, 0xA98));
    }

    character = C_0085e760_GetSearchCharacter(self);
    uint32_t* result = reinterpret_cast<uint32_t*>(character);
    if (CSF_AT(int, character, 0x608) == -1 &&
        CSF_AT(int, character, 0x60C) == -1)
        return result;

    character = C_0085e760_GetSearchCharacter(self);
    result = reinterpret_cast<uint32_t*>(character);
    if (!CSF_AT(int, character, 0x2D8))
        return result;

    character = C_0085e760_GetSearchCharacter(self);
    void* moveTask = character + 0x2D4;
    reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*, float)>(
        CSF_AT(void**, moveTask, 0)[0x2C / 4])(
            moveTask, nullptr, self, deltaTime);

    character = C_0085e760_GetSearchCharacter(self);
    const int taskState = CSF_AT(int, character, 0x2D8);
    result = reinterpret_cast<uint32_t*>(taskState);
    if (taskState != 2 && taskState != 3)
        return result;

    character = C_0085e760_GetSearchCharacter(self);
    void* owner = CSF_AT(void*, character, 0x2C);
    void* actor = CSF_AT(void*, owner, 0x44);
    const bool canFinish = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, actor, 0)[0x90 / 4])(actor, nullptr);
    result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(canFinish));
    if (!canFinish)
        return result;

    character = C_0085e760_GetSearchCharacter(self);
    if (CSF_AT(int, character, 0x2D8) == 3)
    {
        const int linkedTask = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedTask + 12);

        character = C_0085e760_GetSearchCharacter(self);
        const int guardArguments = reinterpret_cast<int>(character + 0x2E8);
        void* lookup = reinterpret_cast<void*>(ResolveEntryLookupValue(self, nullptr));
        auto* provider = reinterpret_cast<C_0085e7b8*>(
            GetCachedObjectProvider(lookup, nullptr));
        GetLinkedObjectField10(self, nullptr);
        return reinterpret_cast<uint32_t*>(
            StaticAi_SetGuardStateFromArgs(provider, guardArguments));
    }

    character = C_0085e760_GetSearchCharacter(self);
    moveTask = character + 0x2D4;
    reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*)>(
        CSF_AT(void**, moveTask, 0)[0x28 / 4])(moveTask, nullptr, self);

    character = C_0085e760_GetSearchCharacter(self);
    CSF_AT(float, character, 0x2E0) = 1.0f;
    CSF_AT(float, character, 0x2E4) = 1.0f;
    return reinterpret_cast<uint32_t*>(character);
}

HOOK(0x004B5370, C_0085e760_UpdateSearch, "auto");
