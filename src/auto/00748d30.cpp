#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int, __fastcall, S_00747f20_GetComparisonStatus, 0x00747F20, (S_00747f20* self, void*));
CSF_FUNC(int, __fastcall, S_00747f20_ComputeStateMode1, 0x007485C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00747f20_GetRelativeSpawnSlotState, 0x00748670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00747f20_ComputeStateMode0, 0x007489A0, (void* self, void* edx));

static int __fastcall S_00747f20_UpdateStateAndCommit(
    S_00747f20* self, void* edx, int /*unused*/)
{
    const int previousState = static_cast<int8_t>(self->field_0300);
    uint8_t state = 0;

    switch (static_cast<int8_t>(self->field_006a))
    {
    case 0:
        state = static_cast<uint8_t>(
            S_00747f20_ComputeStateMode0(self, edx));
        break;
    case 1:
        state = static_cast<uint8_t>(
            S_00747f20_ComputeStateMode1(self, edx));
        break;
    case 2:
        state = static_cast<uint8_t>(
            S_00747f20_GetComparisonStatus(self, edx));
        break;
    case 3:
        state = static_cast<uint8_t>(
            S_00747f20_GetRelativeSpawnSlotState(self, edx));
        break;
    default:
        break;
    }

    self->field_0300 = state;
    const int currentState = static_cast<int8_t>(state);
    if (previousState == currentState)
        return currentState;

    self->field_005c = 3;
    alignas(8) uint8_t spawnRecordBytes[0x8];
    uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(
        reinterpret_cast<uint32_t*>(spawnRecordBytes));
    const int64_t committed = MpSpawn_CommitSpawnRecord(spawnRecord, edx);

    self->field_02e8 = static_cast<uint32_t>(committed);
    self->field_02ec = static_cast<uint32_t>(
        static_cast<uint64_t>(committed) >> 32);
    return static_cast<int32_t>(self->field_02e8);
}

HOOK(0x00748D30, S_00747f20_UpdateStateAndCommit, "auto");
