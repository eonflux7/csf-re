#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAlert_UpdateDistractedLinkedState_4B3430, 0x004B3430, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static int __stdcall AiCoverAttackTasks_UpdateDistractedTask(void* linkedObject, int event)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    void* owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    AiAlert_UpdateDistractedLinkedState_4B3430(owner, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    void* task = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(owner) + 0x258);
    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, task, 0)[0x2C / 4])(
            task, nullptr, linkedObject, event);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    int result = CSF_AT(int, owner, 0x25C);
    if (result != 2 && result != 3)
        return result;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    void* actor = CSF_AT(void*, owner, 0x2C);
    void* status = CSF_AT(void*, actor, 0x44);
    result = ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, status, 0)[0x90 / 4])(status, nullptr);
    if (!result)
        return result;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    if (CSF_AT(int, owner, 0x25C) != 3)
        return Dispatch_NoLinkedTarget(linkedObject, nullptr, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    uint32_t* position = AiCoverAttackTasks_GetPositionData(
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(owner) + 0x290),
        nullptr);

    void* destination = reinterpret_cast<void*>(
        GetLinkedObjectField10(linkedObject, nullptr));
    CSF_AT(uint32_t, destination, 0x1C) = position[0];
    CSF_AT(uint32_t, destination, 0x20) = position[1];
    CSF_AT(uint32_t, destination, 0x24) = position[2];

    int linkedTask = GetLinkedObjectField10(linkedObject, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(
        linkedObject, nullptr, linkedTask + 8);
}

HOOK(0x004B6E60, AiCoverAttackTasks_UpdateDistractedTask, "auto");
