#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiFncStatic_IsGuardTaskComplete, 0x004AA5E0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, AiFncStatic_UpdateGuardCover, 0x004AF160, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_SearchBoundsPadding, 0x00833E20);

[[maybe_unused]] static const auto kGuardTaskVtable = CSF_VTBL(0x004AA5E0);

static void* AiFncStatic_GetOwnerFromLinkedValue(int linkedValue)
{
    return reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
}

static void __stdcall AiFncStatic_UpdateGuardTasks(int characterTask, float deltaTime)
{
    int deltaTimeBits;
    std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));
    AiCoverAttackTasks_UpdateCharacterTasks(characterTask, deltaTimeBits);

    void* task = reinterpret_cast<void*>(characterTask);
    if (static_cast<std::uint8_t>(IsField10Null(task, nullptr)))
        return;

    void* owner = AiFncStatic_GetOwnerFromLinkedValue(
        GetLinkedObjectValue_4DB5C0(task, nullptr));

    if (CSF_AT(std::uint8_t, owner, 0x684))
    {
        void* coverOwner = AiFncStatic_GetOwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        void* stateOwner = AiFncStatic_GetOwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));

        float coverState = static_cast<float>(
            AiFncStatic_UpdateGuardCover(coverOwner, nullptr, deltaTime));
        AiCoverAttackTasks_UpdateCoverState(
            reinterpret_cast<float*>(
                reinterpret_cast<std::uint8_t*>(stateOwner) + 0x690),
            nullptr, coverState);
    }

    bool resetTask = false;
    owner = AiFncStatic_GetOwnerFromLinkedValue(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    if (CSF_AT(int, owner, 0x608) != -1)
    {
        owner = AiFncStatic_GetOwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        if (CSF_AT(std::uint8_t, owner, 0x684))
        {
            owner = AiFncStatic_GetOwnerFromLinkedValue(
                GetLinkedObjectValue_4DB5C0(task, nullptr));
            resetTask = AiFncStatic_IsGuardTaskComplete(owner, nullptr);
        }
    }

    if (!resetTask)
    {
        void* referenceOwner = AiFncStatic_GetOwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));
        void* positionOwner = AiFncStatic_GetOwnerFromLinkedValue(
            GetLinkedObjectValue_4DB5C0(task, nullptr));

        float* position = GetObjectPositionVector(
            CSF_AT(void*, positionOwner, 0x2C), nullptr);
        float dx = position[0] - CSF_AT(float, referenceOwner, 0x64C);
        float dy = position[1] - CSF_AT(float, referenceOwner, 0x650);
        float dz = position[2] - CSF_AT(float, referenceOwner, 0x654);
        resetTask = dz * dz + dx * dx + dy * dy < g_SearchBoundsPadding;
    }

    if (resetTask)
    {
        int linkedTask = GetLinkedObjectField10(task, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedTask + 4);
    }
}

HOOK(0x004AFC70, AiFncStatic_UpdateGuardTasks, "auto");
