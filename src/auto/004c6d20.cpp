#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_IsAttackBlocked, 0x004AA440, (void* a1));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_UpdateAttackState, 0x004AEE90, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_BuildAttackSequence, 0x004C64B0, (int a1));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static void* AiCoverAttackTasks_GetLinkedBase(void* task)
{
    const int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linked) - 4)
        : nullptr;
}

static int __stdcall C_0085e760_UpdateCoverAttackTasks(
    C_0085e760* self, int elapsedTime)
{
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(self), elapsedTime);

    int result = IsField10Null(self, nullptr);
    if (static_cast<uint8_t>(result))
        return result;

    void* linked = AiCoverAttackTasks_GetLinkedBase(self);
    void* owner = CSF_AT(void*, linked, 0x2C);
    void* controller = CSF_AT(void*, owner, 0x48);
    result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x40 / 4])(controller, nullptr);
    if (result)
        return result;

    GetLinkedObjectValue_4DB5C0(self, nullptr);
    result = AiCoverAttackTasks_IsAttackBlocked(self);
    if (static_cast<uint8_t>(result))
        return result;

    linked = AiCoverAttackTasks_GetLinkedBase(self);
    AiCoverAttackTasks_UpdateAttackState(
        linked, nullptr, self, elapsedTime);

    linked = AiCoverAttackTasks_GetLinkedBase(self);
    if (CSF_AT(int, linked, 0x2F8))
    {
        linked = AiCoverAttackTasks_GetLinkedBase(self);
        void* primaryTask = reinterpret_cast<uint8_t*>(linked) + 0x2F4;
        reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*, int)>(
            CSF_AT(void**, primaryTask, 0)[0x2C / 4])(
                primaryTask, nullptr, self, elapsedTime);

        linked = AiCoverAttackTasks_GetLinkedBase(self);
        const int state = CSF_AT(int, linked, 0x2F8);
        if (state == 2 || state == 3)
        {
            linked = AiCoverAttackTasks_GetLinkedBase(self);
            owner = CSF_AT(void*, linked, 0x2C);
            controller = CSF_AT(void*, owner, 0x44);
            const bool ready =
                reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, controller, 0)[0x90 / 4])(
                        controller, nullptr);
            if (ready)
            {
                linked = AiCoverAttackTasks_GetLinkedBase(self);
                if (CSF_AT(int, linked, 0x2F8) == 3)
                {
                    const int linkedTask = GetLinkedObjectField10(self, nullptr);
                    AiCoverAttackTasks_ResetLinkedTaskState(
                        self, nullptr, linkedTask + 4);

                    const int entry = ResolveEntryLookupValue(self, nullptr);
                    void* provider = GetCachedObjectProvider(
                        reinterpret_cast<void*>(entry), nullptr);
                    GetLinkedObjectField10(self, nullptr);
                    AiCoverAttackTasks_BuildAttackSequence(
                        reinterpret_cast<int>(provider));
                }
                else
                {
                    linked = AiCoverAttackTasks_GetLinkedBase(self);
                    CSF_AT(int, linked, 0x2F8) = 1;
                    CSF_AT(uint32_t, linked, 0x300) = 0;
                    CSF_AT(uint32_t, linked, 0x304) = 0;
                    CSF_AT(int, linked, 0x30C) = 0;
                    CSF_AT(int, linked, 0x308) = 3;

                    linked = AiCoverAttackTasks_GetLinkedBase(self);
                    CSF_AT(float, linked, 0x300) = 0.5f;
                    CSF_AT(float, linked, 0x304) = 0.5f;
                }
            }
        }
    }

    linked = AiCoverAttackTasks_GetLinkedBase(self);
    if (CSF_AT(int, linked, 0x314))
    {
        linked = AiCoverAttackTasks_GetLinkedBase(self);
        void* secondaryTask = reinterpret_cast<uint8_t*>(linked) + 0x310;
        reinterpret_cast<void(__fastcall*)(void*, void*, C_0085e760*, int)>(
            CSF_AT(void**, secondaryTask, 0)[0x2C / 4])(
                secondaryTask, nullptr, self, elapsedTime);
    }

    linked = AiCoverAttackTasks_GetLinkedBase(self);
    result = reinterpret_cast<int>(linked);
    if (CSF_AT(uint8_t, linked, 0x6B0))
    {
        linked = AiCoverAttackTasks_GetLinkedBase(self);
        CSF_AT(uint8_t, linked, 0x6B0) = 0;

        const int linkedTask = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedTask + 4);

        const int entry = ResolveEntryLookupValue(self, nullptr);
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(entry), nullptr);
        GetLinkedObjectField10(self, nullptr);
        return AiCoverAttackTasks_BuildAttackSequence(
            reinterpret_cast<int>(provider));
    }
    return result;
}

HOOK(0x004C6D20, C_0085e760_UpdateCoverAttackTasks, "auto");
