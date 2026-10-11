#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_4BC270, 0x004BC270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, sub_4BC4D0, 0x004BC4D0, (void* self, void* edx, float a2, float a3, float a4));
CSF_FUNC(int, __fastcall, sub_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

static uint32_t AiCoverAttackTasks_AppendCommand(
    S_0041e5f0* commands, uint32_t command)
{
    if (commands->field_0008 >= commands->field_0004)
        GrowButtonArray(commands, nullptr, static_cast<int>(commands->field_000c));

    const uint32_t count = commands->field_0008;
    auto* destination = reinterpret_cast<uint32_t*>(
        commands->field_0000 + 4 * count);
    const uint32_t newCount = count + 1;
    commands->field_0008 = newCount;
    *destination = command;
    return newCount;
}

static uint32_t __stdcall AiCoverAttackTasks_BuildCoverAttackCommands(int taskAddress)
{
    void* task = reinterpret_cast<void*>(taskAddress);

    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    uint32_t ownerAddress = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    sub_4DC180(task, nullptr, static_cast<int>(ownerAddress + 1088));

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    ownerAddress = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    auto* commands = reinterpret_cast<S_0041e5f0*>(ownerAddress + 1292);

    AiCoverAttackTasks_AppendCommand(commands, 13);

    const int globalState = GetGlobalState_50D190();
    const uint32_t state = static_cast<uint32_t>(
        GetStateValue(reinterpret_cast<void*>(globalState), nullptr, 100));

    AiCoverAttackTasks_AppendCommand(commands, 10);

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    ownerAddress = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    void* owner = reinterpret_cast<void*>(ownerAddress);
    const uint32_t positionAddress =
        ownerAddress + (CSF_AT(int, owner, 0x630) < 0 ? 1552 : 1576);
    sub_4C5150(commands, nullptr, static_cast<int>(positionAddress));

    float direction;
    if (state < 0x14)
        direction = -1.0f;
    else if (state < 0x28)
        direction = 1.0f;
    else if (state < 0x46)
        direction = -1.0f;
    else
        direction = 1.0f;

    sub_4BC4D0(commands, nullptr, direction, 100.0f, 200.0f);

    if (state >= 0x28)
    {
        sub_4BC270(commands, nullptr);
        AiCoverAttackTasks_AppendCommand(commands, 10);
    }

    sub_4BC3A0(commands, nullptr, 2.0f, 5.0f);
    return AiCoverAttackTasks_AppendCommand(commands, 14);
}

HOOK(0x004C8030, AiCoverAttackTasks_BuildCoverAttackCommands, "auto");
