#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static int __stdcall AiCombat_UpdateLinkedAttackTask(int character, int context)
{
    void* self = reinterpret_cast<void*>(character);

    AiCoverAttackTasks_UpdateCharacterTasks(character, context);
    int result = IsField10Null(self, nullptr);
    if (static_cast<std::uint8_t>(result))
        return result;

    auto* taskFields =
        reinterpret_cast<std::uint8_t*>(GetLinkedObjectField10(self, nullptr)) + 20;
    CSF_AT(int, taskFields, 4) = -1;
    CSF_AT(int, taskFields, 8) = -1;

    int linkedAddress = GetLinkedObjectValue_4DB5C0(self, nullptr);
    result = linkedAddress ? linkedAddress - 4 : 0;
    void* linkedObject = reinterpret_cast<void*>(result);
    if (!CSF_AT(int, linkedObject, 0x2D8))
        return result;

    linkedAddress = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedAddress ? linkedAddress - 4 : 0);
    void* attackTask = reinterpret_cast<std::uint8_t*>(linkedObject) + 0x2D4;
    ((void(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, attackTask, 0)[0x2C / 4])
        (attackTask, nullptr, character, context);

    linkedAddress = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedAddress ? linkedAddress - 4 : 0);
    result = CSF_AT(int, linkedObject, 0x2D8);
    if (result != 2 && result != 3)
        return result;

    linkedAddress = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedAddress ? linkedAddress - 4 : 0);
    void* owner = CSF_AT(void*, linkedObject, 0x2C);
    void* readinessObject = CSF_AT(void*, owner, 0x44);
    bool ready = ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, readinessObject, 0)[0x90 / 4])
        (readinessObject, nullptr);
    if (!ready)
        return static_cast<int>(ready);

    linkedAddress = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedAddress ? linkedAddress - 4 : 0);
    if (CSF_AT(int, linkedObject, 0x2D8) != 3)
        return Dispatch_NoLinkedTarget(self, nullptr, 0);

    linkedAddress = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedAddress ? linkedAddress - 4 : 0);
    taskFields =
        reinterpret_cast<std::uint8_t*>(GetLinkedObjectField10(self, nullptr)) + 20;
    CSF_AT(int, taskFields, 4) = CSF_AT(int, linkedObject, 0x2EC);
    CSF_AT(int, taskFields, 8) = CSF_AT(int, linkedObject, 0x2F0);

    int linkedTaskAddress = GetLinkedObjectField10(self, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedTaskAddress + 8);
}

HOOK(0x004B8240, AiCombat_UpdateLinkedAttackTask, "auto");
