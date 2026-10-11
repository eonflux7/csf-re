#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_SearchBoundsPadding, 0x00833E20);

static void* AiCore_GetLinkedOwner(void* task)
{
    const int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linkedValue
        ? reinterpret_cast<void*>(static_cast<uintptr_t>(
              static_cast<uint32_t>(linkedValue) - 4))
        : nullptr;
}

static int __stdcall AiCore_UpdateLinkedTaskDestination(
    int taskAddress, int /*unused2*/, int /*unused3*/)
{
    void* task = reinterpret_cast<void*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(taskAddress)));

    {
        void* owner = AiCore_GetLinkedOwner(task);
        if (CSF_AT(int, owner, 0x608) == -1)
            goto applyFallback;

        owner = AiCore_GetLinkedOwner(task);
        if (CSF_AT(uint8_t, owner, 0x684))
            goto applyFallback;

        void* destinationOwner = AiCore_GetLinkedOwner(task);
        owner = AiCore_GetLinkedOwner(task);
        const int destinationAddress = static_cast<int>(
            reinterpret_cast<uintptr_t>(destinationOwner) + 0x64C);

        // The supplied binding returns int, but the caller tests only AL.
        if (static_cast<uint8_t>(
                AiCore_IsTargetPositionValid(owner, nullptr, destinationAddress)) == 0)
            goto applyFallback;

        destinationOwner = AiCore_GetLinkedOwner(task);
        owner = AiCore_GetLinkedOwner(task);
        void* position = reinterpret_cast<void*>(static_cast<uintptr_t>(
            static_cast<uint32_t>(GetObjectPositionVector(
                CSF_AT(void*, owner, 0x2C), nullptr))));

        const float dx = CSF_AT(float, position, 0) -
                         CSF_AT(float, destinationOwner, 0x64C);
        const float dy = CSF_AT(float, position, 4) -
                         CSF_AT(float, destinationOwner, 0x650);
        const float dz = CSF_AT(float, position, 8) -
                         CSF_AT(float, destinationOwner, 0x654);
        const float distanceSquared = dz * dz + dx * dx + dy * dy;

        if (!(distanceSquared > g_SearchBoundsPadding))
            goto applyFallback;

        const int linkedState = GetLinkedObjectField10(task, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(
            task, nullptr, static_cast<int>(static_cast<uint32_t>(linkedState) + 8));
    }

applyFallback:
    void* owner = AiCore_GetLinkedOwner(task);
    UnnamedApply_4DC180(
        task, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(owner) + 0x440));

    owner = AiCore_GetLinkedOwner(task);
    return UnnamedSetRangeParams_4BC3A0(
        reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(owner) + 0x50C),
        nullptr, 3.0f, 6.0f);
}

HOOK(0x004BEE60, AiCore_UpdateLinkedTaskDestination, "auto");
