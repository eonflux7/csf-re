#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00747f20_ComputeStateMode0, 0x007489A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00747f20_ComputeStateMode1, 0x007485C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00747f20_ComputeStateMode2, 0x00747F20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00747f20_ComputeStateMode3, 0x00748670, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));

// Reference to S_00747f20's vtable (the original calls its mode-2 method at 0x747f20 and
// the vtable is referenced from the same code region).
[[maybe_unused]] static const void* const s_stateVtableRef = CSF_VTBL(0x00747F20);

static int64_t __fastcall S_00747f20_UpdateStateAndCommit(S_00747f20* self, void* /*edx*/, int a2) {
    (void)a2;  // Argument is popped by the caller convention (ret 4) but not read.

    uint8_t newStateByte = 0;
    int previousState = static_cast<int8_t>(self->field_0300);

    switch (static_cast<int8_t>(self->field_006a)) {
        case 0:
            newStateByte = static_cast<uint8_t>(S_00747f20_ComputeStateMode0(self, nullptr));
            break;
        case 1:
            newStateByte = static_cast<uint8_t>(S_00747f20_ComputeStateMode1(self, nullptr));
            break;
        case 2:
            newStateByte = static_cast<uint8_t>(S_00747f20_ComputeStateMode2(self, nullptr));
            break;
        case 3:
            newStateByte = static_cast<uint8_t>(S_00747f20_ComputeStateMode3(self, nullptr));
            break;
        default:
            break;
    }

    self->field_0300 = newStateByte;
    int64_t result = static_cast<int64_t>(newStateByte);
    int currentState = static_cast<int8_t>(newStateByte);

    if (previousState != currentState) {
        self->field_005c = 3;
        alignas(8) uint8_t fetchBuffer[8];
        uint32_t* spawnRecordArg = reinterpret_cast<uint32_t*>(fetchBuffer);
        uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(spawnRecordArg);
        result = MpSpawn_CommitSpawnRecord(spawnRecord, nullptr);
        self->field_02e8 = static_cast<uint32_t>(result);
        self->field_02ec = static_cast<uint32_t>(static_cast<uint64_t>(result) >> 32);
    }

    return result;
}

HOOK(0x748D30, S_00747f20_UpdateStateAndCommit, "auto");
