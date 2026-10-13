#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_IsAttackBlocked, 0x004AA440, (C_0085e7b8* a1));
CSF_FUNC(char, __fastcall, AiFncStatic_UpdateCoverAttackState, 0x004AEE90, (void* self, void* , C_0085e7b8* actor, int updateArgument));
CSF_FUNC(int, __fastcall, TFncAtacarDesdeParapeto_SelectAttackState, 0x004B6F70, (TFncAtacarDesdeParapeto* self, void* , TAiState* state));
CSF_FUNC(char, __fastcall, AiDistracted_TrySelectNearbyAttackTarget, 0x004B9950, (char* self, void*));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static int __stdcall AiDistracted_UpdateCoverAttackTasks(
    C_0085e760* task, int updateArgument)
{
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(task), updateArgument);

    int result = IsField10Null(task, nullptr);
    if (static_cast<uint8_t>(result))
        return result;

    int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    int characterAddress = linkedValue ? linkedValue - 4 : 0;
    char* character = reinterpret_cast<char*>(characterAddress);
    void* characterObject = CSF_AT(void*, character, 0x2C);
    void* status = CSF_AT(void*, characterObject, 0x48);
    result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, status, 0)[0x40 / 4])(status, nullptr);
    if (result)
        return result;

    GetLinkedObjectValue_4DB5C0(task, nullptr);
    result = AiCoverAttackTasks_IsAttackBlocked(
        reinterpret_cast<C_0085e7b8*>(task));
    if (static_cast<uint8_t>(result))
        return result;

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = reinterpret_cast<char*>(linkedValue ? linkedValue - 4 : 0);
    AiFncStatic_UpdateCoverAttackState(
        character, nullptr, reinterpret_cast<C_0085e7b8*>(task),
        updateArgument);

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = reinterpret_cast<char*>(linkedValue ? linkedValue - 4 : 0);
    if (AiDistracted_TrySelectNearbyAttackTarget(character, nullptr))
    {
        int linkedTask = GetLinkedObjectField10(task, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(
            task, nullptr, linkedTask + 36);
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = reinterpret_cast<char*>(linkedValue ? linkedValue - 4 : 0);
    if (!CSF_AT(uint8_t, character, 0x6B0))
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        result = linkedValue ? linkedValue - 4 : 0;
        character = reinterpret_cast<char*>(result);
        if (!CSF_AT(int, character, 0x2F8))
            return result;

        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        character = reinterpret_cast<char*>(linkedValue ? linkedValue - 4 : 0);
        void* embeddedTask = character + 0x2F4;
        reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*, int)>(
            CSF_AT(void**, embeddedTask, 0)[0x2C / 4])(
                embeddedTask, nullptr, task, updateArgument);

        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        result = linkedValue ? linkedValue - 4 : 0;
        character = reinterpret_cast<char*>(result);
        if (CSF_AT(int, character, 0x2F8) != 3)
            return result;

        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        character = reinterpret_cast<char*>(linkedValue ? linkedValue - 4 : 0);
        CSF_AT(int, character, 0x2F8) = 1;
        CSF_AT(int, character, 0x300) = 0;
        CSF_AT(int, character, 0x304) = 0;
        CSF_AT(int, character, 0x30C) = 0;
        CSF_AT(int, character, 0x308) = 3;

        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        character = reinterpret_cast<char*>(linkedValue ? linkedValue - 4 : 0);
        CSF_AT(float, character, 0x300) = 0.5f;
        CSF_AT(float, character, 0x304) = 0.5f;
    }

    TAiState* state = reinterpret_cast<TAiState*>(
        ResolveEntryLookupValue(task, nullptr));
    TFncAtacarDesdeParapeto* coverAttackTask =
        reinterpret_cast<TFncAtacarDesdeParapeto*>(
            GetLinkedObjectField10(task, nullptr));
    return TFncAtacarDesdeParapeto_SelectAttackState(
        coverAttackTask, nullptr, state);
}

HOOK(0x004BC0C0, AiDistracted_UpdateCoverAttackTasks, "auto");
