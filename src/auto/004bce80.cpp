#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiAttackTasks_StartLinkedObject, 0x004B7DD0, (int object, float duration));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiCore_UpdateLinkedAttackTask(C_0085e760* self, int updateArgument)
{
    AiCoverAttackTasks_UpdateCharacterTasks(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), updateArgument);

    int result = IsField10Null(self, nullptr);
    if (static_cast<std::uint8_t>(result))
        return result;

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    auto* linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    result = static_cast<int>(reinterpret_cast<std::uintptr_t>(linkedObject));
    if (!CSF_AT(int, linkedObject, 0x368))
        return result;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    void* task = linkedObject + 0x364;
    ((void(__fastcall*)(void*, void*, C_0085e760*, int))
        CSF_AT(void**, task, 0)[0x2C / 4])(
            task, nullptr, self, updateArgument);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    result = CSF_AT(int, linkedObject, 0x368);
    if (result != 2 && result != 3)
        return result;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    void* owner = CSF_AT(void*, linkedObject, 0x2C);
    void* controller = CSF_AT(void*, owner, 0x44);
    result = ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, controller, 0)[0x90 / 4])(controller, nullptr);
    if (!result)
        return result;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    if (CSF_AT(int, linkedObject, 0x368) != 3)
        return Dispatch_NoLinkedTarget(self, nullptr, 1);

    int linkedField = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField + 8);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    float duration = CSF_AT(float, linkedObject, 0x384);
    int entry = ResolveEntryLookupValue(self, nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);
    GetLinkedObjectField10(self, nullptr);

    return AiAttackTasks_StartLinkedObject(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(provider)), duration);
}

HOOK(0x004BCE80, AiCore_UpdateLinkedAttackTask, "auto");
