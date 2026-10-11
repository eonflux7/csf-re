#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AppendPosition, 0x004B7150, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AppendCoverData, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, SetLinkedTaskData, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);

// The packet also identifies the growth routine's address as a vtable reference.
static const auto g_ArrayGrowthTableReference = CSF_VTBL(0x0041E5F0);

static void __stdcall AiCoverAttackTasks_BuildCoverAttackPlan(int a1)
{
    void* task = reinterpret_cast<void*>(a1);

    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    uint32_t base = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    SetLinkedTaskData(task, nullptr, static_cast<int>(base + 1088));

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    uint32_t planBase = linked ? static_cast<uint32_t>(linked) - 4 : 0;

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    base = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    uint32_t coverData = base +
        (CSF_AT(int, reinterpret_cast<void*>(base), 0x630) < 0 ? 1552 : 1576);

    auto* commands = reinterpret_cast<S_0041e5f0*>(planBase + 1292);
    AppendCoverData(commands, nullptr, static_cast<int>(coverData));

    if (commands->field_0008 >= commands->field_0004)
        GrowButtonArray(commands, nullptr, commands->field_000c);

    uint32_t count = commands->field_0008;
    auto* slot = reinterpret_cast<uint32_t*>(commands->field_0000) + count;
    commands->field_0008 = count + 1;
    *slot = 7;

    if (commands->field_0008 >= commands->field_0004)
        GrowButtonArray(commands, nullptr, commands->field_000c);

    count = commands->field_0008;
    slot = reinterpret_cast<uint32_t*>(commands->field_0000) + count;
    commands->field_0008 = count + 1;
    *slot = 10;

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    base = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    auto* position = reinterpret_cast<float*>(GetObjectPositionVector(
        CSF_AT(C_008342e8*, reinterpret_cast<void*>(base), 0x2C), nullptr));

    float destination[3] = { position[0], position[1], position[2] };

    int state = GetGlobalState_50D190();
    destination[0] += static_cast<float>(
        GetStateValue(reinterpret_cast<void*>(state), nullptr, 200) - 100);

    state = GetGlobalState_50D190();
    destination[2] += static_cast<float>(
        GetStateValue(reinterpret_cast<void*>(state), nullptr, 200) - 100);

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    base = linked ? static_cast<uint32_t>(linked) - 4 : 0;
    position = reinterpret_cast<float*>(GetObjectPositionVector(
        CSF_AT(C_008342e8*, reinterpret_cast<void*>(base), 0x2C), nullptr));

    float xDistance = destination[0] - position[0];
    float zDistance = destination[2] - position[2];
    if (std::fabs(xDistance) > g_QuarterTurnConstant &&
        std::fabs(zDistance) > g_QuarterTurnConstant)
    {
        if (commands->field_0008 >= commands->field_0004)
            GrowButtonArray(commands, nullptr, commands->field_000c);

        count = commands->field_0008;
        slot = reinterpret_cast<uint32_t*>(commands->field_0000) + count;
        commands->field_0008 = count + 1;
        *slot = 2;

        AppendPosition(commands, nullptr, reinterpret_cast<uint32_t*>(destination));
    }
}

HOOK(0x004C6B70, AiCoverAttackTasks_BuildCoverAttackPlan, "auto");
