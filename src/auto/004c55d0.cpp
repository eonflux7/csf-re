#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_CheckSlotPosition, 0x004B2EE0, (void* self, void* edx, float* a2, int a3));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendPosition, 0x004B7150, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendScalar, 0x004BC2A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendTimeRange, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendCoverPosition, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GridGetSlotPosition, 0x0075D000, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(uint32_t, g_CoverCandidateData, 0x008997E0);
CSF_GLOBAL(uint32_t, g_CoverCandidateCapacity, 0x008997E4);
CSF_GLOBAL(uint32_t, g_CoverCandidateCount, 0x008997E8);
CSF_GLOBAL(int, g_CoverCandidateGrowth, 0x008997EC);

static uint8_t* AiCoverAttackTasks_GetLinkedActor(int reference)
{
    const int linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(reference), nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static uint8_t* AiCoverAttackTasks_GetCoverPosition(uint8_t* actor)
{
    return actor + (CSF_AT(int, actor, 0x630) < 0 ? 0x610 : 0x628);
}

static void S_0041e5f0_AppendWord(S_0041e5f0* array, uint32_t value)
{
    if (array->field_0008 >= array->field_0004)
        GrowButtonArray(array, nullptr, array->field_000c);

    const uint32_t index = array->field_0008;
    auto* destination =
        reinterpret_cast<uint32_t*>(array->field_0000 + 4 * index);
    array->field_0008 = index + 1;
    *destination = value;
}

static char __fastcall S_004bc930_BuildCoverAttackCommands(
    S_004bc930* self, void* /*edx*/, int actorReference)
{
    auto* commands = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x6C);

    uint8_t* actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
    if (!CSF_AT(uint32_t, actor, 0x6C8) ||
        CSF_AT(int, actor, 0x6C0) == -1 ||
        CSF_AT(int, actor, 0x6C4) == -1)
    {
        actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
        uint8_t* coverPosition = AiCoverAttackTasks_GetCoverPosition(actor);
        AiCoverAttackTasks_AppendCoverPosition(
            commands, nullptr, reinterpret_cast<int>(coverPosition));
        S_0041e5f0_AppendWord(commands, 5);
        S_0041e5f0_AppendWord(commands, 10);
        S_0041e5f0_AppendWord(commands, 13);
        S_0041e5f0_AppendWord(commands, 17);
        AiCoverAttackTasks_AppendScalar(commands, nullptr, 1065353216);
        AiCoverAttackTasks_AppendTimeRange(commands, nullptr, 1.0f, 3.0f);
        S_0041e5f0_AppendWord(commands, 14);
        return 1;
    }

    void* grid = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD8);
    actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
    auto* slotPosition = reinterpret_cast<float*>(GridGetSlotPosition(
        grid, nullptr, reinterpret_cast<uint32_t*>(actor + 0x6C0)));

    uint8_t* positionActor = AiCoverAttackTasks_GetLinkedActor(actorReference);
    uint8_t* checkedActor = AiCoverAttackTasks_GetLinkedActor(actorReference);
    uint8_t* coverPosition = AiCoverAttackTasks_GetCoverPosition(positionActor);
    const char usable = AiCoverAttackTasks_CheckSlotPosition(
        checkedActor, nullptr, slotPosition,
        reinterpret_cast<int>(coverPosition));
    if (!usable)
        return usable;

    g_CoverCandidateCount = 0;
    for (uint32_t slot = 0; slot < 5; ++slot)
    {
        actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
        auto* slotData = CSF_AT(uint32_t*, actor, 0x6C8);
        if (slotData[slot] == 1)
        {
            if (g_CoverCandidateCount >= g_CoverCandidateCapacity)
                GrowButtonArray(
                    &g_CoverCandidateData, nullptr, g_CoverCandidateGrowth);

            const uint32_t index = g_CoverCandidateCount;
            auto* destination = reinterpret_cast<uint32_t*>(
                g_CoverCandidateData + 4 * index);
            g_CoverCandidateCount = index + 1;
            *destination = slot;
        }
    }

    const uint32_t candidateCount = g_CoverCandidateCount;
    if (candidateCount)
    {
        actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
        auto* objectPosition = reinterpret_cast<float*>(
            GetObjectPositionVector(CSF_AT(void*, actor, 0x2C), nullptr));

        int nearestCandidate = -1;
        const float dx = slotPosition[0] - objectPosition[0];
        const float dy = slotPosition[1] - objectPosition[1];
        const float dz = slotPosition[2] - objectPosition[2];
        float nearestDistance = dz * dz + dy * dy + dx * dx;

        // The original searches consecutive positions, not the filtered indices.
        int positionOffset = 0;
        for (uint32_t candidate = 0; candidate < candidateCount; ++candidate)
        {
            actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
            auto* slotData = CSF_AT(uint8_t*, actor, 0x6C8);
            const float y = CSF_AT(float, slotData + positionOffset, 0x18)
                          - objectPosition[1];
            const float z = CSF_AT(float, slotData + positionOffset, 0x1C)
                          - objectPosition[2];
            const float x = CSF_AT(float, slotData + positionOffset, 0x14)
                          - objectPosition[0];
            const float distance = x * x + z * z + y * y;
            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestCandidate = static_cast<int>(candidate);
            }
            positionOffset += 12;
        }

        // Reserve the full room available to the original stack object.
        uint32_t destinationPosition[0x24 / sizeof(uint32_t)] = {};
        destinationPosition[0] = CSF_AT(uint32_t, slotPosition, 0);
        destinationPosition[1] = CSF_AT(uint32_t, slotPosition, 4);
        destinationPosition[2] = CSF_AT(uint32_t, slotPosition, 8);

        bool appendStanceCommand = true;
        actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
        if (nearestCandidate == -1)
        {
            if (CSF_AT(uint32_t, CSF_AT(void*, actor, 0x6C8), 0) == 1)
                appendStanceCommand = false;
        }
        else
        {
            const uint32_t slot = reinterpret_cast<uint32_t*>(
                g_CoverCandidateData)[nearestCandidate];
            auto* selectedPosition =
                CSF_AT(uint8_t*, actor, 0x6C8) + 12 * slot + 20;
            destinationPosition[0] = CSF_AT(uint32_t, selectedPosition, 0);
            destinationPosition[1] = CSF_AT(uint32_t, selectedPosition, 4);
            destinationPosition[2] = CSF_AT(uint32_t, selectedPosition, 8);

            if (nearestCandidate == 0 ||
                nearestCandidate == 2 ||
                nearestCandidate == 4)
                appendStanceCommand = false;
        }

        actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
        coverPosition = AiCoverAttackTasks_GetCoverPosition(actor);

        S_0041e5f0_AppendWord(commands, 3);

        auto* records = reinterpret_cast<uint8_t*>(commands) + 0x20;
        if (CSF_AT(uint32_t, records, 8) >= CSF_AT(uint32_t, records, 4))
            MissionScript_GrowEventRecords(
                records, nullptr, CSF_AT(int, records, 0xC));

        const uint32_t recordIndex = CSF_AT(uint32_t, records, 8);
        const uint32_t recordData = CSF_AT(uint32_t, records, 0);
        CSF_AT(uint32_t, records, 8) = recordIndex + 1;
        auto* record = reinterpret_cast<uint8_t*>(recordData + 12 * recordIndex);
        // The first word of this record is deliberately left untouched.
        CSF_AT(uint32_t, record, 4) = CSF_AT(uint32_t, coverPosition, 4);
        CSF_AT(uint32_t, record, 8) = CSF_AT(uint32_t, coverPosition, 8);

        if (appendStanceCommand)
            S_0041e5f0_AppendWord(commands, 6);
        S_0041e5f0_AppendWord(commands, 10);
        S_0041e5f0_AppendWord(commands, 2);
        AiCoverAttackTasks_AppendPosition(commands, nullptr, destinationPosition);
        S_0041e5f0_AppendWord(commands, 8);

        auto* scalars = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(commands) + 0x10);
        S_0041e5f0_AppendWord(scalars, 1065353216);
        return 1;
    }

    actor = AiCoverAttackTasks_GetLinkedActor(actorReference);
    coverPosition = AiCoverAttackTasks_GetCoverPosition(actor);
    AiCoverAttackTasks_AppendCoverPosition(
        commands, nullptr, reinterpret_cast<int>(coverPosition));
    S_0041e5f0_AppendWord(commands, 6);
    S_0041e5f0_AppendWord(commands, 10);
    S_0041e5f0_AppendWord(commands, 2);
    AiCoverAttackTasks_AppendPosition(
        commands, nullptr, reinterpret_cast<uint32_t*>(slotPosition));
    AiCoverAttackTasks_AppendScalar(commands, nullptr, 1065353216);
    return 1;
}

HOOK(0x004C55D0, S_004bc930_BuildCoverAttackCommands, "auto");
