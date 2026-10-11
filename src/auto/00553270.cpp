#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitializeSpawnRecord, 0x004164B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ResolveSpawnParticipant, 0x0073E420,
         (void* self, void* edx, int (__stdcall* identifier)(int)));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetNextSpawnRequest, 0x00741610, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCharacterSelectTime, 0x00748700, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static int __stdcall MpSpawnPoints_ProcessSpawnRequests(int spawnPoint)
{
    void* participant = nullptr;

    // The direction vector occupies the final twelve bytes of this stack buffer.
    struct RecordStorage {
        C_0082e0f4 record;
        float direction[3];
    };
    static_assert(sizeof(RecordStorage) == 0x80);

    for (;;) {
        auto* owner = reinterpret_cast<uint8_t*>(spawnPoint);
        int requestAddress = GetNextSpawnRequest(owner, nullptr);
        if (!requestAddress)
            return requestAddress;

        int targetReference = GetValueIfNotFlagged(owner + 1712, nullptr);
        if (!targetReference)
            continue;

        auto* target = reinterpret_cast<C_008342e8*>(targetReference - 20);
        if (!target)
            continue;

        void* targetComponent = reinterpret_cast<void*>(target->field_0038);
        if (!targetComponent)
            continue;

        if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, targetComponent, 0)[124 / 4])(
                targetComponent, nullptr) != 1)
            continue;

        auto* request = reinterpret_cast<void*>(requestAddress);
        alignas(4) uint8_t command[0x40]{};

        uint32_t requestValue = CSF_AT(uint32_t, request, 0);
        uint32_t requestArgument = CSF_AT(uint32_t, request, 4);
        CSF_AT(int, command, 0x04) = -1;
        CSF_AT(int, command, 0x08) = -1;
        int requestKind = CSF_AT(int8_t, request, 8);
        CSF_AT(uint32_t, command, 0x0C) = requestValue;
        auto identifier = reinterpret_cast<int(__stdcall*)(int)>(
            CSF_AT(uint32_t, request, 12));

        CSF_AT(void**, command, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, command, 0x18) = 0;
        CSF_AT(int, command, 0x1C) = 0;
        CSF_AT(int, command, 0x20) = 0;
        CSF_AT(int, command, 0x24) = 0;
        CSF_AT(int, command, 0x28) = 0;
        CSF_AT(uint32_t, command, 0x10) = requestArgument;
        CSF_AT(int, command, 0x14) = requestKind;

        int registry = GetGlobalDword_73C020();
        int participantReference = ResolveSpawnParticipant(
            reinterpret_cast<void*>(registry), nullptr, identifier);
        if (participantReference)
            participant = reinterpret_cast<void*>(participantReference - 1712);

        uint32_t selectionState = g_CharacterSelectState8B0694;
        uint32_t previousLow = CSF_AT(uint32_t, participant, 2096);
        uint32_t previousHigh = CSF_AT(uint32_t, participant, 2100);
        int currentTime = -1;

        if (selectionState) {
            uint32_t reference = CSF_AT(uint32_t,
                reinterpret_cast<void*>(selectionState), 4);
            if (reference) {
                uint32_t character = reference - 776;
                if (character) {
                    currentTime = GetCharacterSelectTime(
                        reinterpret_cast<void*>(character), nullptr);
                    selectionState = g_CharacterSelectState8B0694;
                }
            }
        }

        uint64_t previousTime =
            (static_cast<uint64_t>(previousHigh) << 32) | previousLow;
        // The original compares both halves unsigned, including the sign-extended time.
        if ((previousHigh || previousLow >= 0x32) &&
            static_cast<uint64_t>(static_cast<int64_t>(currentTime)) >=
                previousTime - 50) {
            CSF_AT(void**, command, 0) = CSF_VTBL(0x0082D410);
            participant = nullptr;
            continue;
        }

        currentTime = -1;
        if (selectionState) {
            uint32_t reference = CSF_AT(uint32_t,
                reinterpret_cast<void*>(selectionState), 4);
            if (reference) {
                uint32_t character = reference - 776;
                if (character)
                    currentTime = GetCharacterSelectTime(
                        reinterpret_cast<void*>(character), nullptr);
            }
        }

        CSF_AT(uint32_t, participant, 2096) = static_cast<uint32_t>(currentTime);
        CSF_AT(uint32_t, participant, 2100) = currentTime < 0 ? UINT32_MAX : 0;

        int sourceReference = GetValueIfNotFlagged(
            reinterpret_cast<uint8_t*>(participant) + 1712, nullptr);
        auto* source = sourceReference
            ? reinterpret_cast<C_008342e8*>(sourceReference - 20)
            : nullptr;

        if (source) {
            CSF_AT(uint32_t, command, 0x04) = source->field_0020;
            CSF_AT(uint32_t, command, 0x08) = source->field_0024;
        } else {
            CSF_AT(void**, command, 0x2C) = CSF_VTBL(0x0082D448);
            CSF_AT(int, command, 0x30) = -1;
            CSF_AT(int, command, 0x34) = -1;
            CSF_AT(uint32_t, command, 0x04) = CSF_AT(uint32_t, command, 0x30);
            CSF_AT(uint32_t, command, 0x08) = CSF_AT(uint32_t, command, 0x34);
            CSF_AT(void**, command, 0x2C) = CSF_VTBL(0x0082D410);
        }

        targetComponent = reinterpret_cast<void*>(target->field_0038);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, targetComponent, 0)[140 / 4])(
                targetComponent, nullptr, command);

        RecordStorage storage;
        InitializeSpawnRecord(&storage.record, nullptr, 4);

        uint32_t sourceId = source->field_0020;
        uint32_t targetId = target->field_0020;
        storage.record.field_000c = sourceId;
        uint32_t sourceGeneration = source->field_0024;
        storage.record.field_001c = target->field_0024;
        storage.record.field_0010 = sourceGeneration;
        storage.record.field_0018 = targetId;

        auto* sourcePosition = reinterpret_cast<float*>(
            GetObjectPositionVector(source, nullptr));
        auto* targetPosition = reinterpret_cast<float*>(
            GetObjectPositionVector(target, nullptr));
        float deltaZ = targetPosition[2] - sourcePosition[2];
        float deltaY = targetPosition[1] - sourcePosition[1];
        storage.direction[0] = targetPosition[0] - sourcePosition[0];
        storage.direction[1] = deltaY;
        storage.direction[2] = deltaZ;

        float* direction = NormalizeVector3(storage.direction, nullptr);
        storage.record.field_004c = CSF_AT(uint32_t, direction, 0);
        storage.record.field_0050 = CSF_AT(uint32_t, direction, 4);
        uint32_t directionZ = CSF_AT(uint32_t, direction, 8);
        uint32_t value = CSF_AT(uint32_t, command, 0x0C);
        storage.record.field_0024 = value;
        storage.record.field_0060 = value;
        storage.record.field_0054 = directionZ;

        int dispatcher = GetGlobalObjectField184();
        DispatchRecordToGlobalObject(
            reinterpret_cast<void*>(dispatcher), nullptr, &storage.record, 1);

        storage.record.field_0014 =
            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
        storage.record.field_0008 =
            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
        storage.record.vtbl = CSF_VTBL(0x0082D410);
        CSF_AT(void**, command, 0) = CSF_VTBL(0x0082D410);
        participant = nullptr;
    }
}

HOOK(0x00553270, MpSpawnPoints_ProcessSpawnRequests, "auto");
