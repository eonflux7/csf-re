#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, AttackTasks_SetCoverAttackPosition, 0x0048CA40, (void* self, void* , const uint32_t* a2));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiAlertStates_SetCoverAttackPositionAndStartAction(C_0085e7b8* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint32_t positionObject = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint32_t taskObject = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;

    uint32_t positionIndex = CSF_AT(uint32_t, reinterpret_cast<void*>(positionObject), 0x638);
    if (positionIndex >= 2u)
        positionIndex = 2;

    void* taskOwner = CSF_AT(void*, reinterpret_cast<void*>(taskObject), 0x2C);
    const uint32_t* coverPosition =
        reinterpret_cast<const uint32_t*>(positionObject + 12 * positionIndex + 0x64C);
    void* task = CSF_AT(void*, taskOwner, 0x204);
    AttackTasks_SetCoverAttackPosition(task, nullptr, coverPosition);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint32_t applyObject = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(applyObject + 0x1AC));

    GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* provider = GetCachedObjectProvider(self, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(provider, nullptr);
    uint32_t actionObject = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(provider, nullptr);
    taskObject = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    taskOwner = CSF_AT(void*, reinterpret_cast<void*>(taskObject), 0x2C);
    task = CSF_AT(void*, taskOwner, 0x204);

    return TaskCharAction_48CFE0(task, nullptr, static_cast<int>(actionObject), 1, 1);
}

HOOK(0x004B1F60, AiAlertStates_SetCoverAttackPositionAndStartAction, "auto");
