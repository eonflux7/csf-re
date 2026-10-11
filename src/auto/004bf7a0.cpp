#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(bool, __fastcall, AiCore_CheckLinkedObjectTarget, 0x004BBAF0, (void* self, void* edx, int a1, int a2));
CSF_FUNC(int, __stdcall, AiCore_ProcessMatchingLinkedTask, 0x004BF740, (int a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also identifies this getter address as a vtable reference.
static const auto kLinkedObjectGetterTable = CSF_VTBL(0x00492B20);

static int __stdcall AiCore_UpdateLinkedTargetTask(
    int taskAddress, int /*unused2*/, int /*unused3*/)
{
    void* task = reinterpret_cast<void*>(taskAddress);

    std::uintptr_t linkedValue =
        static_cast<std::uint32_t>(GetLinkedObjectValue_4DB5C0(task, nullptr));
    auto* linkedObject = reinterpret_cast<std::uint8_t*>(
        linkedValue ? linkedValue - 4 : 0);

    void* descriptor = CSF_AT(void*, linkedObject, 0x2C);
    void* scene = g_pCurrentScene;
    char* target = S_00492b20_GetPtr0x1B4(descriptor, nullptr);
    CHandleTable_Resolve(scene, nullptr, reinterpret_cast<int>(target));

    linkedValue =
        static_cast<std::uint32_t>(GetLinkedObjectValue_4DB5C0(task, nullptr));
    linkedObject = reinterpret_cast<std::uint8_t*>(
        linkedValue ? linkedValue - 4 : 0);

    std::uintptr_t checkerValue =
        static_cast<std::uint32_t>(GetLinkedObjectValue_4DB5C0(task, nullptr));
    auto* checkerObject = reinterpret_cast<std::uint8_t*>(
        checkerValue ? checkerValue - 4 : 0);

    descriptor = CSF_AT(void*, linkedObject, 0x2C);
    target = S_00492b20_GetPtr0x1B4(descriptor, nullptr);
    if (AiCore_CheckLinkedObjectTarget(
            checkerObject + 0x2D4, nullptr, taskAddress,
            reinterpret_cast<int>(target)))
    {
        return AiCore_ProcessMatchingLinkedTask(taskAddress);
    }

    int linkedState = GetLinkedObjectField10(task, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(
        task, nullptr, linkedState + 20);
}

HOOK(0x004BF7A0, AiCore_UpdateLinkedTargetTask, "auto");
