#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiDistracted_AttachTask, 0x004B6140, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static char __stdcall AiDistracted_InitializeTask(int taskAddress)
{
    void* task = reinterpret_cast<void*>(taskAddress);

    int linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    uint32_t actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    void* actor = reinterpret_cast<void*>(actorAddress);
    void* controller = CSF_AT(void*, actor, 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    actor = reinterpret_cast<void*>(actorAddress);
    void* taskData = reinterpret_cast<void*>(
        GetLinkedObjectField10(task, nullptr));

    CSF_AT(uint32_t, taskData, 0x18) = CSF_AT(uint32_t, actor, 0x64C);
    CSF_AT(uint32_t, taskData, 0x1C) = CSF_AT(uint32_t, actor, 0x650);
    CSF_AT(uint32_t, taskData, 0x20) = CSF_AT(uint32_t, actor, 0x654);

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    UnnamedApply_4DC180(task, nullptr, static_cast<int>(actorAddress + 0x21C));

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    void* taskController = reinterpret_cast<void*>(actorAddress + 0x258);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, taskController, 0)[0x28 / 4])(
            taskController, nullptr, taskAddress);

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    actor = reinterpret_cast<void*>(actorAddress);
    CSF_AT(float, actor, 0x264) = 0.3f;
    CSF_AT(float, actor, 0x268) = 0.3f;

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    actor = reinterpret_cast<void*>(actorAddress);
    if (CSF_AT(int, actor, 0x608) == -1)
        return static_cast<char>(actorAddress);

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    actor = reinterpret_cast<void*>(actorAddress);
    void* coverAttack = reinterpret_cast<void*>(actorAddress + 0x5DC);
    uint32_t stateAddress = actorAddress + 0x628;
    if (CSF_AT(int, actor, 0x630) < 0)
        stateAddress = actorAddress + 0x610;

    char stateResult = CoverAttack_CheckState_4ADAE0(
        coverAttack, nullptr, static_cast<int>(stateAddress));
    if (!stateResult)
        return stateResult;

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    AiDistracted_AttachTask(
        reinterpret_cast<void*>(actorAddress + 0x32C), nullptr, taskAddress);

    linkedAddress = GetLinkedObjectValue_4DB5C0(task, nullptr);
    actorAddress = linkedAddress
        ? static_cast<uint32_t>(linkedAddress) - 4 : 0;
    actor = reinterpret_cast<void*>(actorAddress);
    CSF_AT(float, actor, 0x338) = 0.3f;
    CSF_AT(float, actor, 0x33C) = 0.3f;
    return static_cast<char>(actorAddress);
}

HOOK(0x004BAB80, AiDistracted_InitializeTask, "auto");
