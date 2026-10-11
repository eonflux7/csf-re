#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_RegisterPosition, 0x00443B50, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendDelay, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_InitializeSequence, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetMovementDestination, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint8_t* AiCoverAttackTasks_GetLinkedCharacter(void* task)
{
    const int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked
        ? reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(
              static_cast<std::uint32_t>(linked) - 4))
        : nullptr;
}

static int AiCoverAttackTasks_Address(const void* pointer)
{
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(pointer));
}

static void AiCoverAttackTasks_AppendCommand(S_0041e5f0* sequence,
                                            std::uint32_t command)
{
    // The packet labels this call target as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    if (sequence->field_0008 >= sequence->field_0004)
        GrowButtonArray(sequence, nullptr, sequence->field_000c);

    const std::uint32_t index = sequence->field_0008;
    auto* destination = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(sequence->field_0000) + 4 * index);
    sequence->field_0008 = index + 1;
    *destination = command;
}

static int __stdcall AiCoverAttackTasks_InitializeCoveredAttack(void* task)
{
    auto* character = AiCoverAttackTasks_GetLinkedCharacter(task);
    CSF_AT(std::uint32_t, character, 0x6DC) = 0;

    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    auto* actor = CSF_AT(std::uint8_t*, character, 0x2C);
    AiCoverAttackTasks_RegisterPosition(
        CSF_AT(void*, g_pCurrentScene, 0xB8), nullptr,
        AiCoverAttackTasks_Address(actor + 0x1C));

    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    actor = CSF_AT(std::uint8_t*, character, 0x2C);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 0, 0, -1, 0);

    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    CSF_AT(std::uint8_t, character, 0x6B0) = 0;

    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    CSF_AT(std::uint32_t, character, 0x3B8) = 1;
    CSF_AT(float, character, 0x3C0) = 0.0f;
    CSF_AT(float, character, 0x3C4) = 0.0f;
    CSF_AT(float, character, 0x3C0) = 0.1f;
    CSF_AT(float, character, 0x3C4) = 0.1f;

    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    AiCoverAttackTasks_SetMovementDestination(
        task, nullptr, AiCoverAttackTasks_Address(character + 0x440));

    auto* sequenceOwner = AiCoverAttackTasks_GetLinkedCharacter(task);
    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    auto* position = character +
        (CSF_AT(int, character, 0x630) < 0 ? 0x610 : 0x628);
    auto* sequence = reinterpret_cast<S_0041e5f0*>(sequenceOwner + 0x50C);

    AiCoverAttackTasks_InitializeSequence(
        sequence, nullptr, AiCoverAttackTasks_Address(position));
    AiCoverAttackTasks_AppendCommand(sequence, 31);

    const int state = GetGlobalState_50D190();
    const std::uint32_t choice = static_cast<std::uint32_t>(
        GetStateValue(reinterpret_cast<void*>(static_cast<std::uintptr_t>(
                          static_cast<std::uint32_t>(state))),
                      nullptr, 100));
    if (choice < 0x1E)
    {
        AiCoverAttackTasks_AppendCommand(sequence, 15);
        AiCoverAttackTasks_AppendDelay(sequence, nullptr, 1.0f, 3.0f);
        AiCoverAttackTasks_AppendCommand(sequence, 16);
    }
    else
    {
        AiCoverAttackTasks_AppendCommand(sequence, 13);
        AiCoverAttackTasks_AppendDelay(sequence, nullptr, 1.0f, 3.0f);
        AiCoverAttackTasks_AppendCommand(sequence, 14);
    }

    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    CSF_AT(std::uint32_t, character, 0x2F8) = 1;
    CSF_AT(float, character, 0x300) = 0.0f;
    CSF_AT(float, character, 0x304) = 0.0f;
    CSF_AT(std::uint32_t, character, 0x30C) = 0;
    CSF_AT(std::uint32_t, character, 0x308) = 3;

    character = AiCoverAttackTasks_GetLinkedCharacter(task);
    CSF_AT(float, character, 0x300) = 0.5f;
    CSF_AT(float, character, 0x304) = 0.5f;
    return AiCoverAttackTasks_Address(character);
}

HOOK(0x004C9330, AiCoverAttackTasks_InitializeCoveredAttack, "auto");
