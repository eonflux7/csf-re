#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, AppendDurationRange, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AppendCoverPosition, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, SetTaskPosition, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

// The packet also classifies the array-growth call address as a vtable reference.
[[maybe_unused]] static const auto kArrayGrowthReference = CSF_VTBL(0x0041E5F0);

static int AiCoverAttackTasks_AppendCommand(S_0041e5f0* commands, uint32_t command)
{
    if (commands->field_0008 >= commands->field_0004)
        GrowButtonArray(commands, nullptr, static_cast<int>(commands->field_000c));

    const uint32_t count = commands->field_0008;
    auto* destination = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(commands->field_0000) + 4 * count);
    const uint32_t nextCount = count + 1;
    commands->field_0008 = nextCount;
    *destination = command;
    return static_cast<int>(nextCount);
}

static int __stdcall AiCoverAttackTasks_BuildCoverAttackCommands(void* task)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    uintptr_t linkedBase = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;
    SetTaskPosition(task, nullptr, static_cast<int>(linkedBase + 1088));

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    const uintptr_t ownerBase = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    linkedBase = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;

    const uintptr_t coverPosition =
        CSF_AT(int, reinterpret_cast<void*>(linkedBase), 0x630) < 0
        ? linkedBase + 1552
        : linkedBase + 1576;

    auto* commands = reinterpret_cast<S_0041e5f0*>(ownerBase + 1292);
    AppendCoverPosition(commands, nullptr, static_cast<int>(coverPosition));

    AiCoverAttackTasks_AppendCommand(commands, 10);
    AiCoverAttackTasks_AppendCommand(commands, 0);

    const int globalState = GetGlobalState_50D190();
    const uint32_t stateValue = static_cast<uint32_t>(
        GetStateValue(reinterpret_cast<void*>(globalState), nullptr, 100));

    if (stateValue < 0x1E)
    {
        AiCoverAttackTasks_AppendCommand(commands, 15);
        AppendDurationRange(commands, nullptr, 1.0f, 3.0f);
        AiCoverAttackTasks_AppendCommand(commands, 16);
    }
    else
    {
        AiCoverAttackTasks_AppendCommand(commands, 13);
        AppendDurationRange(commands, nullptr, 1.0f, 3.0f);
        AiCoverAttackTasks_AppendCommand(commands, 14);
    }

    return AiCoverAttackTasks_AppendCommand(commands, 1);
}

HOOK(0x004C69C0, AiCoverAttackTasks_BuildCoverAttackCommands, "auto");
