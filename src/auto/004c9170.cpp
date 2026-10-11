#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendPosition, 0x004B7150, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendScalarBits, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendRange, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendCoverReference, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AssignLinkedTaskContext, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static uint8_t* AiCoverAttackTasks_GetLinkedOwner(void* task)
{
    const uint32_t linked = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    return reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(linked ? linked - 4 : 0));
}

static void AiCoverAttackTasks_AppendCommand(void* commands, uint32_t command)
{
    if (CSF_AT(uint32_t, commands, 0x8) >= CSF_AT(uint32_t, commands, 0x4))
    {
        GrowButtonArray(commands, nullptr, CSF_AT(int, commands, 0xC));
    }

    const uint32_t count = CSF_AT(uint32_t, commands, 0x8);
    uint32_t* destination = CSF_AT(uint32_t*, commands, 0x0) + count;
    CSF_AT(uint32_t, commands, 0x8) = count + 1;
    *destination = command;
}

static int __stdcall AiCoverAttackTasks_BuildCoverAttack(
    void* task, uint32_t* targetPosition)
{
    void* system = reinterpret_cast<void*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(GetValue8B0554())));
    uint8_t* dispatcher = CSF_AT(uint8_t*, system, 0x1FD8);

    uint8_t* owner = AiCoverAttackTasks_GetLinkedOwner(task);
    uint8_t* source = CSF_AT(uint8_t*, owner, 0x2C) + 28;

    uint32_t arguments[7];
    arguments[0] = 2;
    arguments[1] = static_cast<uint32_t>(
        reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    arguments[2] = CSF_AT(uint32_t, source, 0x4);
    arguments[3] = CSF_AT(uint32_t, source, 0x8);
    arguments[4] = targetPosition[0];
    arguments[5] = targetPosition[1];
    arguments[6] = targetPosition[2];
    DispatchWithArgBlock(dispatcher + 68, nullptr, arguments);

    owner = AiCoverAttackTasks_GetLinkedOwner(task);
    AiCoverAttackTasks_AssignLinkedTaskContext(
        task, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(owner) + 1088));

    uint8_t* commandOwner = AiCoverAttackTasks_GetLinkedOwner(task);
    uint8_t* coverOwner = AiCoverAttackTasks_GetLinkedOwner(task);
    uint8_t* coverReference = coverOwner +
        (CSF_AT(int, coverOwner, 0x630) < 0 ? 1552 : 1576);

    void* commands = commandOwner + 1292;
    AiCoverAttackTasks_AppendCoverReference(
        commands, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(coverReference)));

    AiCoverAttackTasks_AppendCommand(commands, 5);
    AiCoverAttackTasks_AppendCommand(commands, 10);
    AiCoverAttackTasks_AppendCommand(commands, 13);
    AiCoverAttackTasks_AppendCommand(commands, 22);
    AiCoverAttackTasks_AppendPosition(commands, nullptr, targetPosition);
    AiCoverAttackTasks_AppendRange(commands, nullptr, 2.0f, 4.0f);
    AiCoverAttackTasks_AppendCommand(commands, 14);
    return AiCoverAttackTasks_AppendScalarBits(commands, nullptr, 1065353216);
}

HOOK(0x004C9170, AiCoverAttackTasks_BuildCoverAttack, "auto");
