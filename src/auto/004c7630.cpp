#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendResetCommand, 0x004BC270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendParameterCommand, 0x004BC340, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendPrimaryAction, 0x004BC4D0, (void* self, void* edx, float a2, float a3, float a4));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendSecondaryAction, 0x004BC570, (void* self, void* edx, float a2, float a3, float a4));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendPositionCommand, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetMovementTarget, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

static std::uint32_t AiCoverAttackTasks_GetActorAddress(void* task)
{
    const int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? static_cast<std::uint32_t>(linked) - 4 : 0;
}

static std::uint32_t AiCoverAttackTasks_AppendCommand(
    S_0041e5f0* commands, std::uint32_t command)
{
    // The packet also lists this direct-call address as a vtable reference.
    // There is no corresponding vtable write in the original.
    (void)CSF_VTBL(0x0041E5F0);

    if (commands->field_0008 >= commands->field_0004)
        GrowButtonArray(commands, nullptr, commands->field_000c);

    const std::uint32_t count = commands->field_0008;
    auto* entry = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(commands->field_0000) + 4 * count);
    const std::uint32_t nextCount = count + 1;
    commands->field_0008 = nextCount;
    *entry = command;
    return nextCount;
}

static std::uint32_t __stdcall AiCoverAttackTasks_BuildAttackSequence(int taskAddress)
{
    void* task = reinterpret_cast<void*>(static_cast<std::uintptr_t>(taskAddress));

    const std::uint32_t initialActor = AiCoverAttackTasks_GetActorAddress(task);
    AiCoverAttackTasks_SetMovementTarget(
        task, nullptr, static_cast<int>(initialActor + 1088));

    const std::uint32_t actor = AiCoverAttackTasks_GetActorAddress(task);
    const int globalState = GetGlobalState_50D190();
    const int attackPattern = GetStateValue(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(globalState)),
        nullptr, 9);

    auto* commands = reinterpret_cast<S_0041e5f0*>(
        static_cast<std::uintptr_t>(actor + 1292));

    switch (attackPattern)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        break;
    case 7:
        AiCoverAttackTasks_AppendResetCommand(commands, nullptr);
        break;
    default:
        AiCoverAttackTasks_AppendCommand(commands, 7);
        break;
    }

    AiCoverAttackTasks_AppendCommand(commands, 10);

    const std::uint32_t positionActor = AiCoverAttackTasks_GetActorAddress(task);
    void* positionOwner = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(positionActor));
    const std::uint32_t positionAddress =
        positionActor + (CSF_AT(int, positionOwner, 0x630) < 0 ? 1552 : 1576);
    AiCoverAttackTasks_AppendPositionCommand(
        commands, nullptr, static_cast<int>(positionAddress));

    AiCoverAttackTasks_AppendCommand(commands, 13);

    switch (attackPattern)
    {
    case 0:
    case 1:
    {
        const float direction = attackPattern == 0 ? -1.0f : 1.0f;
        AiCoverAttackTasks_AppendSecondaryAction(
            commands, nullptr, direction, 100.0f, 400.0f);
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, 1.0f, 100.0f, 400.0f);
        AiCoverAttackTasks_AppendParameterCommand(commands, nullptr, 0x40000000);

        AiCoverAttackTasks_AppendCommand(commands, 14);
        AiCoverAttackTasks_AppendResetCommand(commands, nullptr);
        AiCoverAttackTasks_AppendCommand(commands, 10);
        AiCoverAttackTasks_AppendCommand(commands, 13);

        AiCoverAttackTasks_AppendParameterCommand(commands, nullptr, 0x40000000);
        AiCoverAttackTasks_AppendSecondaryAction(
            commands, nullptr, direction, 100.0f, 400.0f);
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, -1.0f, 100.0f, 400.0f);
        break;
    }
    case 2:
        AiCoverAttackTasks_AppendSecondaryAction(
            commands, nullptr, -1.0f, 100.0f, 200.0f);
        AiCoverAttackTasks_AppendParameterCommand(commands, nullptr, 0x40000000);
        AiCoverAttackTasks_AppendSecondaryAction(
            commands, nullptr, -1.0f, 100.0f, 200.0f);
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, 1.0f, 100.0f, 200.0f);
        AiCoverAttackTasks_AppendParameterCommand(commands, nullptr, 0x40000000);
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, 1.0f, 100.0f, 200.0f);

        AiCoverAttackTasks_AppendCommand(commands, 14);
        AiCoverAttackTasks_AppendResetCommand(commands, nullptr);
        AiCoverAttackTasks_AppendCommand(commands, 10);
        AiCoverAttackTasks_AppendCommand(commands, 13);

        AiCoverAttackTasks_AppendSecondaryAction(
            commands, nullptr, 1.0f, 100.0f, 400.0f);
        return AiCoverAttackTasks_AppendCommand(commands, 14);

    case 3:
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, -1.0f, 100.0f, 500.0f);
        break;

    case 4:
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, 1.0f, 100.0f, 500.0f);
        break;

    case 5:
    case 6:
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, attackPattern == 5 ? -1.0f : 1.0f,
            100.0f, 500.0f);
        AiCoverAttackTasks_AppendCommand(commands, 14);
        AiCoverAttackTasks_AppendResetCommand(commands, nullptr);
        AiCoverAttackTasks_AppendCommand(commands, 10);
        AiCoverAttackTasks_AppendCommand(commands, 13);
        break;

    case 7:
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, 1.0f, 100.0f, 500.0f);
        AiCoverAttackTasks_AppendParameterCommand(commands, nullptr, 0x40000000);
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, -1.0f, 100.0f, 500.0f);
        break;

    default:
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, -1.0f, 100.0f, 500.0f);
        AiCoverAttackTasks_AppendParameterCommand(commands, nullptr, 0x40000000);
        AiCoverAttackTasks_AppendPrimaryAction(
            commands, nullptr, 1.0f, 100.0f, 500.0f);
        break;
    }

    AiCoverAttackTasks_AppendParameterCommand(commands, nullptr, 0x40000000);
    return AiCoverAttackTasks_AppendCommand(commands, 14);
}

HOOK(0x004C7630, AiCoverAttackTasks_BuildAttackSequence, "auto");
