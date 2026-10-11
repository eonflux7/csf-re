#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendVector, 0x004B7150, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendPosition, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(bool, __fastcall, AiCoverAttackTasks_TestCoverPosition, 0x004E5C70, (void* self, void* edx, float* arg0, int a2, int a4));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t*, g_CoverCandidateData, 0x008997E0);
CSF_GLOBAL(uint32_t, g_CoverCandidateCapacity, 0x008997E4);
CSF_GLOBAL(uint32_t, g_CoverCandidateCount, 0x008997E8);
CSF_GLOBAL(int, g_CoverCandidateGrowth, 0x008997EC);

// The packet labels the array-growth call address as a vtable reference.
[[maybe_unused]] static const auto g_ArrayGrowthAddressTag = CSF_VTBL(0x0041E5F0);

static uint8_t* AiCoverAttackTasks_GetLinkedBase(int object)
{
    const int linked = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(object), nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static void AiCoverAttackTasks_AppendCandidate(uint32_t candidate)
{
    if (g_CoverCandidateCount >= g_CoverCandidateCapacity)
        GrowButtonArray(&g_CoverCandidateData, nullptr, g_CoverCandidateGrowth);

    uint32_t* destination = g_CoverCandidateData + g_CoverCandidateCount;
    ++g_CoverCandidateCount;
    *destination = candidate;
}

static void S_004bc930_AppendSequenceWord(S_004bc930* self, uint32_t word)
{
    if (self->field_0074 >= CSF_AT(uint32_t, self, 0x70))
    {
        GrowButtonArray(
            reinterpret_cast<uint8_t*>(self) + 0x6C,
            nullptr,
            CSF_AT(int, self, 0x78));
    }

    uint32_t* destination =
        CSF_AT(uint32_t*, self, 0x6C) + self->field_0074;
    ++self->field_0074;
    *destination = word;
}

static char __fastcall S_004bc930_BuildCoverAttackSequence(
    S_004bc930* self, void* /*edx*/, int object)
{
    g_CoverCandidateCount = 0;

    uint8_t* linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
    if (CSF_AT(uint32_t, linkedBase, 0x6C8) &&
        CSF_AT(int, linkedBase, 0x6C0) != -1 &&
        CSF_AT(int, linkedBase, 0x6C4) != -1)
    {
        int entryOffset = 0;
        for (uint32_t candidate = 0; candidate < 5; ++candidate, entryOffset += 12)
        {
            linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
            if (CSF_AT(uint32_t*, linkedBase, 0x6C8)[candidate] != 2)
                continue;

            linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
            uint8_t* position = linkedBase +
                (CSF_AT(int, linkedBase, 0x630) < 0 ? 0x610 : 0x628);

            if (CSF_AT(int, position, 0x08) < 0)
            {
                AiCoverAttackTasks_AppendCandidate(candidate);
            }
            else
            {
                uint8_t* positionBase = AiCoverAttackTasks_GetLinkedBase(object);
                uint8_t* entriesBase = AiCoverAttackTasks_GetLinkedBase(object);
                uint8_t* contextBase = AiCoverAttackTasks_GetLinkedBase(object);

                position = positionBase +
                    (CSF_AT(int, positionBase, 0x630) < 0 ? 0x610 : 0x628);
                uint8_t* entries = CSF_AT(uint8_t*, entriesBase, 0x6C8);
                void* context = CSF_AT(void*, contextBase, 0x2C);
                void* tester = CSF_AT(void*, context, 0x3C);

                if (AiCoverAttackTasks_TestCoverPosition(
                        tester, nullptr,
                        reinterpret_cast<float*>(entries + entryOffset + 20),
                        reinterpret_cast<int>(position), -1))
                {
                    AiCoverAttackTasks_AppendCandidate(candidate);
                }
            }
        }
    }

    if (!g_CoverCandidateCount)
    {
        S_004bc930_AppendSequenceWord(self, 31);
        S_004bc930_AppendSequenceWord(self, 5);
        return 1;
    }

    const uint32_t candidateCount = g_CoverCandidateCount;
    const int state = GetGlobalState_50D190();
    const int randomIndex = GetStateValue(
        reinterpret_cast<void*>(state), nullptr,
        static_cast<int>(candidateCount));
    int selected = g_CoverCandidateData[randomIndex];

    linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
    bool preferSecond = false;
    if (CSF_AT(uint32_t*, linkedBase, 0x6C8)[2] == 2)
    {
        linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
        preferSecond = CSF_AT(uint32_t*, linkedBase, 0x6C8)[1] == 1;
    }

    if (preferSecond)
    {
        selected = 2;
    }
    else
    {
        linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
        if (CSF_AT(uint32_t*, linkedBase, 0x6C8)[4] == 2)
        {
            linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
            if (CSF_AT(uint32_t*, linkedBase, 0x6C8)[3] == 1)
                selected = 4;
        }
    }

    linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
    uint8_t* position = linkedBase +
        (CSF_AT(int, linkedBase, 0x630) < 0 ? 0x610 : 0x628);
    void* sequence = reinterpret_cast<uint8_t*>(self) + 0x6C;

    AiCoverAttackTasks_AppendPosition(
        sequence, nullptr, reinterpret_cast<int>(position));
    S_004bc930_AppendSequenceWord(self, 6);

    linkedBase = AiCoverAttackTasks_GetLinkedBase(object);
    uint32_t* selectedPosition = reinterpret_cast<uint32_t*>(
        CSF_AT(uint8_t*, linkedBase, 0x6C8) + 12 * selected + 20);

    S_004bc930_AppendSequenceWord(self, 2);
    AiCoverAttackTasks_AppendVector(sequence, nullptr, selectedPosition);
    S_004bc930_AppendSequenceWord(self, 31);

    if (!selected || selected == 2 || selected == 4)
        S_004bc930_AppendSequenceWord(self, 5);

    return 1;
}

HOOK(0x004C5230, S_004bc930_BuildCoverAttackSequence, "auto");
