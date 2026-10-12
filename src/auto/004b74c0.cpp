#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static char __stdcall AiStateFactory_PopPendingTaskAndReset(int request)
{
    void* self = reinterpret_cast<void*>(request);
    auto* selectedTask = reinterpret_cast<std::uint8_t*>(
        GetLinkedObjectField10(self, nullptr)) + 172;
    auto* linkedObject = reinterpret_cast<std::uint8_t*>(
        GetLinkedObjectField10(self, nullptr));

    const std::uint32_t firstTask =
        CSF_AT(std::uint32_t, CSF_AT(void*, linkedObject, 116), 0);
    auto* pendingTasks = linkedObject + 100;
    CSF_AT(std::uint32_t, selectedTask, 0) = firstTask;

    const std::uint32_t remaining =
        CSF_AT(std::uint32_t, pendingTasks, 24) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, pendingTasks, 24) = 0;
    }
    else
    {
        CSF_AT(std::uint32_t, pendingTasks, 24) = remaining;
        void* entries = CSF_AT(void*, pendingTasks, 16);
        _memcpy(entries, static_cast<std::uint8_t*>(entries) + 4,
                static_cast<std::size_t>(4 * remaining));
    }

    if (CSF_AT(std::uint8_t,
               reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr)),
               176))
    {
        GetLinkedObjectField10(self, nullptr);
    }
    GetLinkedObjectField10(self, nullptr);
    const int linked = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linked + 24);
    return 1;
}

HOOK(0x004B74C0, AiStateFactory_PopPendingTaskAndReset, "auto");
