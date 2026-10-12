#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(bool, __fastcall, CoverTask_IsLinkedObjectReady, 0x004AF3D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static std::uint8_t* AiFncStatic_GetLinkedObjectBase(void* task)
{
    const int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    if (!linkedValue)
        return nullptr;

    return reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(linkedValue) - 4));
}

static void __stdcall AiFncStatic_UpdateLinkedCoverTask(void* task, float elapsedTime)
{
    std::uint8_t* linkedObject = AiFncStatic_GetLinkedObjectBase(task);
    CSF_AT(float, linkedObject, 0x69C) -= elapsedTime;

    linkedObject = AiFncStatic_GetLinkedObjectBase(task);
    if (CoverTask_IsLinkedObjectReady(linkedObject, nullptr))
    {
        linkedObject = AiFncStatic_GetLinkedObjectBase(task);
        if (!CSF_AT(std::uint8_t, linkedObject, 0x6CC))
        {
            Dispatch_NoLinkedTarget(task, nullptr, 0);
            return;
        }
    }
    else
    {
        linkedObject = AiFncStatic_GetLinkedObjectBase(task);
        if (!(CSF_AT(float, linkedObject, 0x69C) < g_fZero))
            return;

        linkedObject = AiFncStatic_GetLinkedObjectBase(task);
        if (CSF_AT(std::uint8_t, linkedObject, 0x6CC))
        {
            linkedObject = AiFncStatic_GetLinkedObjectBase(task);
            void* embeddedTask = linkedObject + 0x2A4;
            ((void(__fastcall*)(void*, void*, void*, float))
                CSF_AT(void**, embeddedTask, 0)[0x2C / 4])(
                    embeddedTask, nullptr, task, elapsedTime);

            linkedObject = AiFncStatic_GetLinkedObjectBase(task);
            if (CSF_AT(int, linkedObject, 0x2A8) != 3)
                return;
        }
        else
        {
            const int linkedState = GetLinkedObjectField10(task, nullptr);
            AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedState + 4);
            return;
        }
    }

    const int linkedState = GetLinkedObjectField10(task, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedState + 16);
}

HOOK(0x004B11F0, AiFncStatic_UpdateLinkedCoverTask, "auto");
