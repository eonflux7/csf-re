#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

static int __stdcall AiFncSearch_SelectLinkedTaskState(
    void* task, int /*unused2*/, int /*unused3*/)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    auto* linkedObject = reinterpret_cast<std::uint8_t*>(
        linkedValue ? linkedValue - 4 : 0);

    if (CSF_AT(int, linkedObject, 0x608) != -1)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        linkedObject = reinterpret_cast<std::uint8_t*>(
            linkedValue ? linkedValue - 4 : 0);

        auto* coverState = linkedObject + 0x5DC;
        auto* stateEntry = linkedObject + 0x628;
        if (CSF_AT(int, linkedObject, 0x630) < 0)
            stateEntry = coverState + 0x34;

        if (CoverAttack_CheckState_4ADAE0(
                coverState, nullptr, reinterpret_cast<int>(stateEntry)))
        {
            const int linkedTask = GetLinkedObjectField10(task, nullptr);
            return AiCoverAttackTasks_ResetLinkedTaskState(
                task, nullptr, linkedTask + 8);
        }
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (static_cast<std::uint32_t>(
            GetStateValue(globalState, nullptr, 100)) >= 0x32u)
    {
        const int linkedTask = GetLinkedObjectField10(task, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(
            task, nullptr, linkedTask + 20);
    }

    const int linkedTask = GetLinkedObjectField10(task, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(
        task, nullptr, linkedTask + 12);
}

HOOK(0x004B5100, AiFncSearch_SelectLinkedTaskState, "auto");
