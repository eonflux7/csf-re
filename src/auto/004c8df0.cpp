#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_44A950, 0x0044A950, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4BC2A0, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, sub_4BC430, 0x004BC430, (void* self, void* edx, float a2, int a3));
CSF_FUNC(int, __fastcall, sub_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet labels the direct growth-call address as a vtable reference.
[[maybe_unused]] static const auto kArrayGrowthReference = CSF_VTBL(0x0041E5F0);

static std::uint8_t* CoverAttackTasks_GetLinkedObjectBase(void* link)
{
    const int value = GetLinkedObjectValue_4DB5C0(link, nullptr);
    return reinterpret_cast<std::uint8_t*>(
        value ? static_cast<std::uint32_t>(value) - 4u : 0u);
}

static void CoverAttackTasks_AppendOpcode(S_0041e5f0* sequence, std::uint32_t opcode)
{
    const std::uint32_t index = sequence->field_0008;
    auto* destination =
        reinterpret_cast<std::uint32_t*>(sequence->field_0000) + index;
    sequence->field_0008 = index + 1;
    *destination = opcode;
}

static int __stdcall CoverAttackTasks_InitializeAttackSequence(int taskAddress)
{
    void* task = reinterpret_cast<void*>(taskAddress);

    auto* actor = CoverAttackTasks_GetLinkedObjectBase(task);
    CSF_AT(std::uint32_t, actor, 0x6DC) = 0;

    actor = CoverAttackTasks_GetLinkedObjectBase(task);
    void* actorObject = CSF_AT(void*, actor, 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actorObject, 0)[0x8C / 4])(
        actorObject, nullptr, 0, 0, -1, 0);

    actor = CoverAttackTasks_GetLinkedObjectBase(task);
    sub_4DC180(task, nullptr, reinterpret_cast<int>(actor + 0x440));

    auto* sequenceOwner = CoverAttackTasks_GetLinkedObjectBase(task);
    actor = CoverAttackTasks_GetLinkedObjectBase(task);
    auto* position = actor +
        (CSF_AT(int, actor, 0x630) < 0 ? 0x610 : 0x628);
    auto* sequence = reinterpret_cast<S_0041e5f0*>(sequenceOwner + 0x50C);
    sub_4C5150(sequence, nullptr, reinterpret_cast<int>(position));

    if (sequence->field_0008 >= sequence->field_0004)
        GrowButtonArray(sequence, nullptr, sequence->field_000c);
    CoverAttackTasks_AppendOpcode(sequence, 5);

    if (sequence->field_0008 >= sequence->field_0004)
        GrowButtonArray(sequence, nullptr, sequence->field_000c);
    CoverAttackTasks_AppendOpcode(sequence, 10);

    actor = CoverAttackTasks_GetLinkedObjectBase(task);
    auto* linkedActor = CoverAttackTasks_GetLinkedObjectBase(actor + 8);
    auto* linkedObject = CSF_AT(std::uint8_t*, linkedActor, 0x2C);
    void* sceneQuery = CSF_AT(void*, g_pCurrentScene, 0xC4);
    const int positionType =
        sub_44A950(sceneQuery, nullptr, reinterpret_cast<int>(linkedObject + 0x1C));

    bool usePositiveOffset;
    if (positionType == 3)
        usePositiveOffset = true;
    else if (positionType == 4)
        usePositiveOffset = false;
    else
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        usePositiveOffset = GetStateValue(state, nullptr, 2) == 0;
    }

    float offset;
    if (usePositiveOffset)
    {
        actor = CoverAttackTasks_GetLinkedObjectBase(task);
        CSF_AT(std::uint32_t, actor, 0x6DC) = 3;
        if (sequence->field_0008 >= sequence->field_0004)
            GrowButtonArray(sequence, nullptr, sequence->field_000c);
        offset = 200.0f;
    }
    else
    {
        actor = CoverAttackTasks_GetLinkedObjectBase(task);
        CSF_AT(std::uint32_t, actor, 0x6DC) = 4;
        if (sequence->field_0008 >= sequence->field_0004)
            GrowButtonArray(sequence, nullptr, sequence->field_000c);
        offset = -200.0f;
    }

    CoverAttackTasks_AppendOpcode(sequence, 13);
    sub_4BC430(sequence, nullptr, offset, 1145569280);
    sub_4BC3A0(sequence, nullptr, 2.0f, 5.0f);

    if (sequence->field_0008 >= sequence->field_0004)
        GrowButtonArray(sequence, nullptr, sequence->field_000c);
    CoverAttackTasks_AppendOpcode(sequence, 14);
    return sub_4BC2A0(sequence, nullptr, 1065353216);
}

HOOK(0x004C8DF0, CoverAttackTasks_InitializeAttackSequence, "auto");
