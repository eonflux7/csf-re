#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_IsCoverActionComplete, 0x004AA110, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_IsMovementBlocked, 0x004AA5A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiFncStatic_UpdateCoverAttackState, 0x004AEE90, (void* self, void* , C_0085e7b8* actor, int updateArgument));
CSF_FUNC(void, __fastcall, AiDistracted_UpdateCharacterReaction, 0x004B3430, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiDistracted_RestartCoverAction, 0x004B6140, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, AiCore_ActivateAssignedCoverState, 0x004BA3C0, (void* a1, char a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static void* AiDistracted_GetLinkedCharacter(C_0085e760* actor)
{
    const int linked = GetLinkedObjectValue_4DB5C0(actor, nullptr);
    return linked ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linked) - 4)
                  : nullptr;
}

static void* AiDistracted_MemberAddress(void* object, std::uintptr_t offset)
{
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(object) + offset);
}

static int __fastcall AiDistracted_UpdateCharacterState(
    void* self, void* /*edx*/, C_0085e760* actor, float deltaTime)
{
    int updateArgument;
    std::memcpy(&updateArgument, &deltaTime, sizeof(updateArgument));

    AiCoverAttackTasks_UpdateCharacterTasks(reinterpret_cast<int>(actor), updateArgument);

    int result = IsField10Null(actor, nullptr);
    if (static_cast<std::uint8_t>(result))
        return result;

    GetLinkedObjectValue_4DB5C0(actor, nullptr);
    if (GetCachedObjectProvider(actor, nullptr))
    {
        void* provider = GetCachedObjectProvider(actor, nullptr);
        void* collection = reinterpret_cast<void*>(
            SceneLoader_GetWorldObjectCollection(provider, nullptr));
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, collection, 0)[0x1C / 4])(collection, nullptr);
        if (result == 32)
            return result;
    }

    void* character = AiDistracted_GetLinkedCharacter(actor);
    bool blocked = false;
    if (CSF_AT(std::uint8_t, character, 0x684))
    {
        character = AiDistracted_GetLinkedCharacter(actor);
        blocked = CSF_AT(int, character, 0x608) != -1;
    }
    if (!blocked)
    {
        character = AiDistracted_GetLinkedCharacter(actor);
        blocked = AiCoverAttackTasks_IsMovementBlocked(character, nullptr);
    }

    if (blocked)
    {
        GetLinkedObjectValue_4DB5C0(actor, nullptr);
        character = AiDistracted_GetLinkedCharacter(actor);
        void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        AiCoverAttackTasks_SetCoverTransition(
            AiDistracted_MemberAddress(character, 0x690), nullptr,
            0, CSF_AT(int, globalState, 0xA98));
    }
    else
    {
        character = AiDistracted_GetLinkedCharacter(actor);
        // These two code addresses are also tagged as vtables in the packet.
        (void)CSF_VTBL(0x0047AE10);
        (void)CSF_VTBL(0x0048C9F0);

        char* targetName = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, character, 0x718), nullptr);
        ResetState(targetName, nullptr);

        void* owner = CSF_AT(void*, character, 0x2C);
        alignas(8) std::uint8_t targetHandleBytes[0x10];
        CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, targetHandleBytes, 0x4) = -1;
        CSF_AT(int, targetHandleBytes, 0x8) = -1;
        AiAttackTasks_CopyTargetHandle(
            CSF_AT(void*, owner, 0x204), nullptr,
            reinterpret_cast<int>(targetHandleBytes));

        character = AiDistracted_GetLinkedCharacter(actor);
        AiCoverAttackTasks_UpdateCoverState(
            reinterpret_cast<float*>(AiDistracted_MemberAddress(character, 0x690)),
            nullptr, deltaTime);
    }

    if (CSF_AT(std::uint8_t, self, 0x4))
    {
        character = AiDistracted_GetLinkedCharacter(actor);
        AiDistracted_UpdateCharacterReaction(character, nullptr);
    }

    character = AiDistracted_GetLinkedCharacter(actor);
    result = reinterpret_cast<int>(character);
    if (CSF_AT(int, character, 0x608) == -1 &&
        CSF_AT(int, character, 0x60C) == -1)
        return result;

    character = AiDistracted_GetLinkedCharacter(actor);
    if (CSF_AT(int, character, 0x608) != -1 ||
        CSF_AT(int, character, 0x60C) != -1)
    {
        character = AiDistracted_GetLinkedCharacter(actor);
        if (CSF_AT(int, character, 0x330))
        {
            character = AiDistracted_GetLinkedCharacter(actor);
            void* action = AiDistracted_MemberAddress(character, 0x32C);
            reinterpret_cast<void (__fastcall*)(void*, void*, C_0085e760*, float)>(
                CSF_AT(void**, action, 0)[0x2C / 4])(
                    action, nullptr, actor, deltaTime);

            character = AiDistracted_GetLinkedCharacter(actor);
            action = AiDistracted_MemberAddress(character, 0x32C);
            if (static_cast<std::uint8_t>(
                    AiCoverAttackTasks_IsCoverActionComplete(action, nullptr)))
            {
                character = AiDistracted_GetLinkedCharacter(actor);
                void* owner = CSF_AT(void*, character, 0x2C);
                void* controller = CSF_AT(void*, owner, 0x44);
                if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0x90 / 4])(
                            controller, nullptr))
                {
                    character = AiDistracted_GetLinkedCharacter(actor);
                    bool activateAssignedState = false;
                    if (CSF_AT(int, character, 0x330) == 3)
                    {
                        character = AiDistracted_GetLinkedCharacter(actor);
                        activateAssignedState =
                            CSF_AT(int, character, 0x350) != 0;
                    }

                    if (activateAssignedState)
                    {
                        const int linkedTask = GetLinkedObjectField10(actor, nullptr);
                        AiCoverAttackTasks_ResetLinkedTaskState(
                            actor, nullptr, linkedTask + 4);
                        void* entry = reinterpret_cast<void*>(
                            ResolveEntryLookupValue(actor, nullptr));
                        void* provider = GetCachedObjectProvider(entry, nullptr);
                        GetLinkedObjectField10(actor, nullptr);
                        AiCore_ActivateAssignedCoverState(provider, 0);
                    }
                    else
                    {
                        character = AiDistracted_GetLinkedCharacter(actor);
                        AiDistracted_RestartCoverAction(
                            AiDistracted_MemberAddress(character, 0x32C),
                            nullptr, reinterpret_cast<int>(actor));
                        character = AiDistracted_GetLinkedCharacter(actor);
                        CSF_AT(int, character, 0x338) = 1050253722;
                        CSF_AT(int, character, 0x33C) = 1050253722;
                    }
                }
            }
        }
    }

    character = AiDistracted_GetLinkedCharacter(actor);
    return AiFncStatic_UpdateCoverAttackState(
        AiDistracted_MemberAddress(character, 0x32C), nullptr,
        reinterpret_cast<C_0085e7b8*>(actor), updateArgument);
}

HOOK(0x004BA850, AiDistracted_UpdateCharacterState, "auto");
