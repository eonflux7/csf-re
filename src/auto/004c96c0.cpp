#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_QueueAttackPositionActions, 0x004C9170, (int a1, uint32_t* arg4));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateTask, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SelectLinkedStateFunction, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static void* C_0085e760_GetLinkedBase(C_0085e760* self)
{
    const uint32_t linkedValue =
        static_cast<uint32_t>(GetLinkedObjectValue_4DB5C0(self, nullptr));
    return linkedValue ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
}

static uint32_t* __stdcall C_0085e760_UpdateCoverAttack(
    C_0085e760* self, int updateArgument)
{
    AiCoverAttackTasks_UpdateTask(
        reinterpret_cast<int>(self), updateArgument);

    const int missingLink = IsField10Null(self, nullptr);
    if (static_cast<uint8_t>(missingLink))
        return reinterpret_cast<uint32_t*>(missingLink);

    void* linkedBase = C_0085e760_GetLinkedBase(self);
    if (!CSF_AT(uint32_t, linkedBase, 0x25C))
        return static_cast<uint32_t*>(linkedBase);

    for (unsigned int iteration = 0; iteration < 5; ++iteration)
    {
        linkedBase = C_0085e760_GetLinkedBase(self);
        const uint32_t state = CSF_AT(uint32_t, linkedBase, 0x25C);
        if (state == 2 || state == 3)
            break;

        linkedBase = C_0085e760_GetLinkedBase(self);
        void* controller = reinterpret_cast<void*>(
            reinterpret_cast<uintptr_t>(linkedBase) + 0x258);
        ((void(__fastcall*)(void*, void*, C_0085e760*, int))
            CSF_AT(void**, controller, 0)[0x2C / 4])(
                controller, nullptr, self, updateArgument);
    }

    linkedBase = C_0085e760_GetLinkedBase(self);
    const uint32_t state = CSF_AT(uint32_t, linkedBase, 0x25C);
    if (state != 2 && state != 3)
        return reinterpret_cast<uint32_t*>(state);

    linkedBase = C_0085e760_GetLinkedBase(self);
    void* character = CSF_AT(void*, linkedBase, 0x2C);
    void* status = CSF_AT(void*, character, 0x44);
    const bool ready = ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, status, 0)[0x90 / 4])(status, nullptr);
    if (!ready)
        return nullptr;

    linkedBase = C_0085e760_GetLinkedBase(self);
    if (CSF_AT(uint32_t, linkedBase, 0x25C) == 3)
    {
        const uint32_t linkedField =
            static_cast<uint32_t>(GetLinkedObjectField10(self, nullptr));
        AiCoverAttackTasks_SelectLinkedStateFunction(
            self, nullptr, static_cast<int>(linkedField + 0x18));

        linkedBase = C_0085e760_GetLinkedBase(self);
        uint32_t* position = AiCoverAttackTasks_GetPositionData(
            reinterpret_cast<void*>(
                reinterpret_cast<uintptr_t>(linkedBase) + 0x290),
            nullptr);

        void* entry = reinterpret_cast<void*>(
            ResolveEntryLookupValue(self, nullptr));
        void* provider = GetCachedObjectProvider(entry, nullptr);

        GetLinkedObjectField10(self, nullptr);
        return reinterpret_cast<uint32_t*>(
            AiCoverAttackTasks_QueueAttackPositionActions(
                reinterpret_cast<int>(provider), position));
    }

    linkedBase = C_0085e760_GetLinkedBase(self);
    void* controller = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(linkedBase) + 0x258);
    ((void(__fastcall*)(void*, void*, C_0085e760*))
        CSF_AT(void**, controller, 0)[0x28 / 4])(
            controller, nullptr, self);

    linkedBase = C_0085e760_GetLinkedBase(self);
    CSF_AT(uint8_t, linkedBase, 0x26D) = 0;

    linkedBase = C_0085e760_GetLinkedBase(self);
    CSF_AT(float, linkedBase, 0x264) = 0.1f;
    CSF_AT(float, linkedBase, 0x268) = 0.1f;
    return static_cast<uint32_t*>(linkedBase);
}

HOOK(0x004C96C0, C_0085e760_UpdateCoverAttack, "auto");
