#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, AiFncStatic_IsGuardTaskComplete, 0x004AA5E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static const auto kGuardTaskTypeBinding = CSF_VTBL(0x004AA5E0);

static char __stdcall AiAlert_UpdateAndResetCompletedGuardTask(int task, int updateContext)
{
    AiCoverAttackTasks_UpdateCharacterTasks(task, updateContext);

    void* taskObject = reinterpret_cast<void*>(task);
    char result = static_cast<char>(IsField10Null(taskObject, nullptr));
    if (result)
        return result;

    int linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    auto* linkedObject = linkedValue
        ? reinterpret_cast<S_004aa5e0*>(static_cast<uintptr_t>(linkedValue) - 4)
        : nullptr;

    if (linkedObject->field_0608 == static_cast<uint32_t>(-1))
        return static_cast<char>(reinterpret_cast<uintptr_t>(linkedObject));

    linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<S_004aa5e0*>(static_cast<uintptr_t>(linkedValue) - 4)
        : nullptr;

    void* coverState = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(linkedObject) + 0x5DC);
    void* stateArgument = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(linkedObject) + 0x628);
    if (CSF_AT(int, coverState, 0x54) < 0)
        stateArgument = reinterpret_cast<void*>(
            reinterpret_cast<uintptr_t>(coverState) + 0x34);

    result = CoverAttack_CheckState_4ADAE0(
        coverState, nullptr, reinterpret_cast<int>(stateArgument));
    if (!result)
        return result;

    linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<S_004aa5e0*>(static_cast<uintptr_t>(linkedValue) - 4)
        : nullptr;

    if (!linkedObject->field_0684)
        return static_cast<char>(reinterpret_cast<uintptr_t>(linkedObject));

    linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<S_004aa5e0*>(static_cast<uintptr_t>(linkedValue) - 4)
        : nullptr;

    result = static_cast<char>(
        AiFncStatic_IsGuardTaskComplete(linkedObject, nullptr));
    if (!result)
        return result;

    int linkedField = GetLinkedObjectField10(taskObject, nullptr);
    return static_cast<char>(AiCoverAttackTasks_ResetLinkedTaskState(
        taskObject, nullptr,
        static_cast<int>(static_cast<uint32_t>(linkedField) + 4)));
}

HOOK(0x004B4F70, AiAlert_UpdateAndResetCompletedGuardTask, "auto");
