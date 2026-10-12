#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, ConfigureLinkedObject, 0x004B2030, (int arg0, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiAlert_UpdateDistractedLinkedState_4B3430, 0x004B3430, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static void* AiCombat_GetLinkedCombatObject(C_0085e760* entry)
{
    const int linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    return linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
}

static uint32_t* __stdcall AiCombat_UpdateLinkedCombatTask(
    C_0085e760* entry, int updateArgument)
{
    void* combat = AiCombat_GetLinkedCombatObject(entry);
    AiAlert_UpdateDistractedLinkedState_4B3430(combat, nullptr);

    combat = AiCombat_GetLinkedCombatObject(entry);
    if (CSF_AT(int, combat, 0x608) == -1 &&
        CSF_AT(int, combat, 0x60C) == -1)
    {
        return reinterpret_cast<uint32_t*>(
            Dispatch_NoLinkedTarget(entry, nullptr, 0));
    }

    combat = AiCombat_GetLinkedCombatObject(entry);
    if (CSF_AT(int, combat, 0x608) == -1 &&
        CSF_AT(int, combat, 0x60C) == -1)
    {
        return reinterpret_cast<uint32_t*>(combat);
    }

    combat = AiCombat_GetLinkedCombatObject(entry);
    if (!CSF_AT(int, combat, 0x330))
        return reinterpret_cast<uint32_t*>(combat);

    combat = AiCombat_GetLinkedCombatObject(entry);
    void* task = reinterpret_cast<uint8_t*>(combat) + 0x32C;
    reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*, int)>(
        CSF_AT(void**, task, 0)[0x2C / 4])(
            task, nullptr, entry, updateArgument);

    combat = AiCombat_GetLinkedCombatObject(entry);
    const int taskState = CSF_AT(int, combat, 0x330);
    if (taskState != 2 && taskState != 3)
        return reinterpret_cast<uint32_t*>(taskState);

    combat = AiCombat_GetLinkedCombatObject(entry);
    void* owner = CSF_AT(void*, combat, 0x2C);
    void* status = CSF_AT(void*, owner, 0x44);
    const bool ready = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, status, 0)[0x90 / 4])(status, nullptr);
    if (!ready)
        return nullptr;

    combat = AiCombat_GetLinkedCombatObject(entry);
    if (CSF_AT(int, combat, 0x330) == 3)
    {
        combat = AiCombat_GetLinkedCombatObject(entry);
        if (CSF_AT(int, combat, 0x350))
        {
            const int linkedTask = GetLinkedObjectField10(entry, nullptr);
            AiCoverAttackTasks_ResetLinkedTaskState(
                entry, nullptr, linkedTask + 32);

            combat = AiCombat_GetLinkedCombatObject(entry);
            uint32_t* configuration = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(combat) + 0x358);

            const int lookup = ResolveEntryLookupValue(entry, nullptr);
            void* provider = GetCachedObjectProvider(
                reinterpret_cast<void*>(lookup), nullptr);

            GetLinkedObjectField10(entry, nullptr);
            return reinterpret_cast<uint32_t*>(ConfigureLinkedObject(
                reinterpret_cast<int>(provider), configuration));
        }
    }

    const int linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    const int state = linkedValue
        ? CSF_AT(int, reinterpret_cast<void*>(linkedValue), 0x32C)
        : CSF_AT(int, static_cast<void*>(nullptr), 0x330);
    return reinterpret_cast<uint32_t*>(
        Dispatch_NoLinkedTarget(entry, nullptr, state != 3));
}

HOOK(0x004B7EF0, AiCombat_UpdateLinkedCombatTask, "auto");
