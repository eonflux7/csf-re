#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendTimingRange, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendSignedParameters, 0x004BC4D0, (void* self, void* edx, float a2, float a3, float a4));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendInitialParameters, 0x004BC570, (void* self, void* edx, float a2, float a3, float a4));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendPosition, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetStateData, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

static uint32_t __stdcall AiCoverAttackTasks_BuildRandomizedCoverAttackSequence(int taskAddress)
{
    void* task = reinterpret_cast<void*>(taskAddress);

    int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    uint32_t ownerAddress = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;
    AiCoverAttackTasks_SetStateData(
        task, nullptr, static_cast<int>(ownerAddress + 1088));

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    ownerAddress = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;

    int globalState = GetGlobalState_50D190();
    uint32_t stateValue = static_cast<uint32_t>(
        GetStateValue(reinterpret_cast<void*>(globalState), nullptr, 100));

    auto* sequence = reinterpret_cast<S_0041e5f0*>(ownerAddress + 1292);
    uint32_t capacity = sequence->field_0004;
    bool positiveDirection = stateValue < 0x32;

    if (sequence->field_0008 >= capacity)
        GrowButtonArray(sequence, nullptr, sequence->field_000c);

    uint32_t index = sequence->field_0008;
    auto* entry = reinterpret_cast<uint32_t*>(sequence->field_0000 + 4 * index);
    sequence->field_0008 = index + 1;
    *entry = 10;

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    ownerAddress = linkedValue
        ? static_cast<uint32_t>(linkedValue) - 4
        : 0;
    uint32_t positionAddress =
        CSF_AT(int, reinterpret_cast<void*>(ownerAddress), 0x630) < 0
        ? ownerAddress + 1552
        : ownerAddress + 1576;
    AiCoverAttackTasks_AppendPosition(
        sequence, nullptr, static_cast<int>(positionAddress));

    if (sequence->field_0008 >= sequence->field_0004)
        GrowButtonArray(sequence, nullptr, sequence->field_000c);

    index = sequence->field_0008;
    entry = reinterpret_cast<uint32_t*>(sequence->field_0000 + 4 * index);
    sequence->field_0008 = index + 1;
    *entry = 13;

    AiCoverAttackTasks_AppendInitialParameters(
        sequence, nullptr, -1.0f, 100.0f, 200.0f);
    AiCoverAttackTasks_AppendSignedParameters(
        sequence, nullptr, positiveDirection ? 1.0f : -1.0f, 100.0f, 200.0f);
    AiCoverAttackTasks_AppendTimingRange(sequence, nullptr, 2.0f, 5.0f);

    if (sequence->field_0008 >= sequence->field_0004)
        GrowButtonArray(sequence, nullptr, sequence->field_000c);

    index = sequence->field_0008;
    entry = reinterpret_cast<uint32_t*>(sequence->field_0000 + 4 * index);
    uint32_t result = index + 1;
    sequence->field_0008 = result;
    *entry = 14;
    return result;
}

HOOK(0x004C7450, AiCoverAttackTasks_BuildRandomizedCoverAttackSequence, "auto");
