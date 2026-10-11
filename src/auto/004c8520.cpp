#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_CheckAttackCondition, 0x004AA440, (void* a1));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_UpdateAttack, 0x004AEE90, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_IsMovementComplete, 0x004B9950, (void* self, void* edx));
CSF_FUNC(uint32_t, __stdcall, AiCoverAttackTasks_BuildArrivalCommands, 0x004C7450, (int a1));
CSF_FUNC(uint32_t, __stdcall, AiCoverAttackTasks_BuildAttackCommands, 0x004C7630, (int a1));
CSF_FUNC(uint32_t, __stdcall, AiCoverAttackTasks_BuildCoverAttackCommands, 0x004C8030, (int a1));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateBaseTask, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SelectTaskEntry, 0x004DC380, (void* self, void* edx, int a2));

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_SearchBoundsPadding, 0x00833E20);

static void* AiCoverAttackTasks_OwnerFromLinkedValue(int linked)
{
    return reinterpret_cast<void*>(
        linked ? static_cast<uint32_t>(linked) - 4 : 0);
}

static uint32_t __stdcall AiCoverAttackTasks_UpdateCoverAttack(
    C_0085e760* task, int updateArgument)
{
    AiCoverAttackTasks_UpdateBaseTask(
        reinterpret_cast<int>(task), updateArgument);

    uint32_t result = IsField10Null(task, nullptr);
    if (static_cast<uint8_t>(result))
        return result;

    void* owner = AiCoverAttackTasks_OwnerFromLinkedValue(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    void* object = CSF_AT(void*, owner, 0x2C);
    void* controller = CSF_AT(void*, object, 0x48);
    result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x40 / 4])(controller, nullptr);
    if (result)
        return result;

    GetLinkedObjectValue_4DB5C0(task, nullptr);
    result = AiCoverAttackTasks_CheckAttackCondition(task);
    if (static_cast<uint8_t>(result))
        return result;

    owner = AiCoverAttackTasks_OwnerFromLinkedValue(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    AiCoverAttackTasks_UpdateAttack(owner, nullptr, task, updateArgument);

    owner = AiCoverAttackTasks_OwnerFromLinkedValue(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    if (CSF_AT(int, owner, 0x608) != -1)
    {
        void* positionOwner = AiCoverAttackTasks_OwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        void* destinationOwner = AiCoverAttackTasks_OwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        void* position = reinterpret_cast<void*>(GetObjectPositionVector(
            CSF_AT(void*, positionOwner, 0x2C), nullptr));

        const float dx = CSF_AT(float, destinationOwner, 0x64C)
                       - CSF_AT(float, position, 0);
        const float dy = CSF_AT(float, destinationOwner, 0x650)
                       - CSF_AT(float, position, 4);
        const float dz = CSF_AT(float, destinationOwner, 0x654)
                       - CSF_AT(float, position, 8);

        if (dz * dz + dy * dy + dx * dx < g_SearchBoundsPadding)
        {
            int entry = GetLinkedObjectField10(task, nullptr);
            AiCoverAttackTasks_SelectTaskEntry(task, nullptr, entry + 4);
            void* lookup = reinterpret_cast<void*>(
                ResolveEntryLookupValue(task, nullptr));
            void* provider = GetCachedObjectProvider(lookup, nullptr);
            GetLinkedObjectField10(task, nullptr);
            AiCoverAttackTasks_BuildArrivalCommands(
                reinterpret_cast<int>(provider));
        }
    }

    owner = AiCoverAttackTasks_OwnerFromLinkedValue(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    if (CSF_AT(uint8_t, owner, 0x6B0))
    {
        int entry = GetLinkedObjectField10(task, nullptr);
        AiCoverAttackTasks_SelectTaskEntry(task, nullptr, entry + 4);
        void* lookup = reinterpret_cast<void*>(
            ResolveEntryLookupValue(task, nullptr));
        void* provider = GetCachedObjectProvider(lookup, nullptr);
        GetLinkedObjectField10(task, nullptr);
        AiCoverAttackTasks_BuildCoverAttackCommands(
            reinterpret_cast<int>(provider));

        owner = AiCoverAttackTasks_OwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        CSF_AT(uint8_t, owner, 0x6B0) = 0;
        return reinterpret_cast<uint32_t>(owner);
    }

    owner = AiCoverAttackTasks_OwnerFromLinkedValue(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    if (CSF_AT(int, owner, 0x2F8))
    {
        owner = AiCoverAttackTasks_OwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        void* movement = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t>(owner) + 0x2F4);
        reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*, int)>(
            CSF_AT(void**, movement, 0)[0x2C / 4])(
                movement, nullptr, task, updateArgument);

        owner = AiCoverAttackTasks_OwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        result = reinterpret_cast<uint32_t>(owner);
        if (CSF_AT(int, owner, 0x2F8) == 3)
        {
            owner = AiCoverAttackTasks_OwnerFromLinkedValue(
                GetLinkedObjectValue_4DB5C0(task, nullptr));
            CSF_AT(int, owner, 0x2F8) = 1;
            CSF_AT(int, owner, 0x300) = 0;
            CSF_AT(int, owner, 0x304) = 0;
            CSF_AT(int, owner, 0x30C) = 0;
            CSF_AT(int, owner, 0x308) = 3;

            owner = AiCoverAttackTasks_OwnerFromLinkedValue(
                GetLinkedObjectValue_4DB5C0(task, nullptr));
            CSF_AT(float, owner, 0x300) = 0.5f;
            CSF_AT(float, owner, 0x304) = 0.5f;

            int entry = GetLinkedObjectField10(task, nullptr);
            AiCoverAttackTasks_SelectTaskEntry(task, nullptr, entry + 4);
            void* lookup = reinterpret_cast<void*>(
                ResolveEntryLookupValue(task, nullptr));
            void* provider = GetCachedObjectProvider(lookup, nullptr);
            GetLinkedObjectField10(task, nullptr);
            return AiCoverAttackTasks_BuildAttackCommands(
                reinterpret_cast<int>(provider));
        }
    }
    else
    {
        owner = AiCoverAttackTasks_OwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        result = static_cast<uint8_t>(
            AiCoverAttackTasks_IsMovementComplete(owner, nullptr));
        if (static_cast<uint8_t>(result))
        {
            int entry = GetLinkedObjectField10(task, nullptr);
            return AiCoverAttackTasks_SelectTaskEntry(
                task, nullptr, entry + 12);
        }
    }
    return result;
}

HOOK(0x004C8520, AiCoverAttackTasks_UpdateCoverAttack, "auto");
