#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCore_UpdateLinkedTask, 0x004B1420, (void* self, void* edx, int a3, int a4, char a5));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall C_0085e760_UpdateMacroOrder(C_0085e760* self, int deltaBits)
{
    float delta;
    std::memcpy(&delta, &deltaBits, sizeof(delta));

    void* task = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));
    CSF_AT(float, task, 0xC0) = delta + CSF_AT(float, task, 0xC0);

    int entry = ResolveEntryLookupValue(self, nullptr);
    task = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));
    AiCore_UpdateLinkedTask(task, nullptr, entry, deltaBits, 0);

    void* elapsedTask = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));
    void* limitTask = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));
    int result;

    if (CSF_AT(float, elapsedTask, 0xC0) > CSF_AT(float, limitTask, 0xAC))
    {
        task = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));
        if (!CSF_AT(uint8_t, task, 0xB0))
            goto resetTask;

        int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        uint8_t* owner = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;
        void* actor = CSF_AT(void*, CSF_AT(void*, owner, 0x2C), 0x44);
        if (((bool(__fastcall*)(void*, void*))CSF_AT(void**, actor, 0)[0x90 / 4])(
                actor, nullptr))
            goto resetTask;
    }

    result = GetLinkedObjectField10(self, nullptr);
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xB0))
    {
        int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        uint8_t* owner = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;
        void* actor = CSF_AT(void*, CSF_AT(void*, owner, 0x2C), 0x44);

        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        owner = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;
        result = CSF_AT(int, CSF_AT(void*, owner, 0x718), 0x2E8);
        if (!result)
        {
            void* items =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, actor, 0)[0xDC / 4])(
                    actor, nullptr);
            result = GetCurrentItemCount(items, nullptr);
            if (!result)
                goto resetTask;
        }
    }
    return result;

resetTask:
    int linkedTask = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedTask + 4);
    int providerEntry = ResolveEntryLookupValue(self, nullptr);
    GetCachedObjectProvider(reinterpret_cast<void*>(providerEntry), nullptr);
    return GetLinkedObjectField10(self, nullptr);
}

HOOK(0x004B1860, C_0085e760_UpdateMacroOrder, "auto");
