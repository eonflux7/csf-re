#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, S_004aa5e0_IsGuardTaskComplete, 0x004AA5E0, (S_004aa5e0* self, void*));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

[[maybe_unused]] static auto const kGuardTaskVtable = CSF_VTBL(0x004AA5E0);

static uint8_t* AiFncSearch_AdjustLinkedObject(int linkedValue)
{
    return reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0);
}

static char __stdcall AiFncSearch_UpdateSearchTasks(int character, int context)
{
    void* characterObject = reinterpret_cast<void*>(character);
    AiCoverAttackTasks_UpdateCharacterTasks(character, context);

    char result = static_cast<char>(IsField10Null(characterObject, nullptr));
    if (result)
        return result;

    uint8_t* linked = AiFncSearch_AdjustLinkedObject(
        GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
    if (CSF_AT(int, linked, 0x608) == -1)
        return static_cast<char>(reinterpret_cast<uintptr_t>(linked));

    linked = AiFncSearch_AdjustLinkedObject(
        GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
    uint8_t* coverState = linked + 0x5DC;
    uint8_t* statePosition = linked + 0x628;
    if (CSF_AT(int, coverState, 0x54) < 0)
        statePosition = coverState + 0x34;

    result = CoverAttack_CheckState_4ADAE0(
        coverState, nullptr, reinterpret_cast<int>(statePosition));
    if (!result)
        return result;

    linked = AiFncSearch_AdjustLinkedObject(
        GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
    if (CSF_AT(int, linked, 0x25C))
    {
        linked = AiFncSearch_AdjustLinkedObject(
            GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
        void* movementTask = linked + 0x258;
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, movementTask, 0)[0x2C / 4])(
                movementTask, nullptr, character, context);

        linked = AiFncSearch_AdjustLinkedObject(
            GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
        int taskState = CSF_AT(int, linked, 0x25C);
        if (taskState == 2 || taskState == 3)
        {
            linked = AiFncSearch_AdjustLinkedObject(
                GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
            void* owner = CSF_AT(void*, linked, 0x2C);
            void* actor = CSF_AT(void*, owner, 0x44);
            if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, actor, 0)[0x90 / 4])(actor, nullptr))
            {
                linked = AiFncSearch_AdjustLinkedObject(
                    GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
                if (CSF_AT(int, linked, 0x25C) == 3)
                {
                    int linkedTask = GetLinkedObjectField10(characterObject, nullptr);
                    AiCoverAttackTasks_ResetLinkedTaskState(
                        characterObject, nullptr, linkedTask + 8);
                }
                else
                {
                    linked = AiFncSearch_AdjustLinkedObject(
                        GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
                    movementTask = linked + 0x258;
                    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, movementTask, 0)[0x28 / 4])(
                            movementTask, nullptr, character);

                    linked = AiFncSearch_AdjustLinkedObject(
                        GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
                    CSF_AT(float, linked, 0x264) = 0.3f;
                    CSF_AT(float, linked, 0x268) = 0.3f;
                }
            }
        }
    }

    linked = AiFncSearch_AdjustLinkedObject(
        GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
    if (!CSF_AT(uint8_t, linked, 0x684))
        return static_cast<char>(reinterpret_cast<uintptr_t>(linked));

    linked = AiFncSearch_AdjustLinkedObject(
        GetLinkedObjectValue_4DB5C0(characterObject, nullptr));
    result = static_cast<char>(S_004aa5e0_IsGuardTaskComplete(
        reinterpret_cast<S_004aa5e0*>(linked), nullptr));
    if (result)
    {
        int linkedTask = GetLinkedObjectField10(characterObject, nullptr);
        result = static_cast<char>(AiCoverAttackTasks_ResetLinkedTaskState(
            characterObject, nullptr, linkedTask + 8));
    }
    return result;
}

HOOK(0x004B51B0, AiFncSearch_UpdateSearchTasks, "auto");
