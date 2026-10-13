#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void* , int newActionMode));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiCoverTasks_ResetLinkedActorCoverState(void* task)
{
    std::uintptr_t linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    void* actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    CSF_AT(std::uint32_t, actor, 0x6DC) = 0;

    linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    CSF_AT(std::uint8_t, actor, 0x6B0) = 0;

    linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    CSF_AT(std::uint8_t, actor, 0x6CC) = 0;

    linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    void* controller = CSF_AT(void*, actor, 0x2C);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    actor = reinterpret_cast<void*>(linked ? linked - 4 : 0);
    return C_00834050_SetActionMode(
        CSF_AT(void*, actor, 0x2C), nullptr, 1);
}

HOOK(0x004AC320, AiCoverTasks_ResetLinkedActorCoverState, "auto");
