#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetNextPendingSpawnRecord, 0x0073FCB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

// Stack message built by the original: vtable, two -1 ids, a record copy, a size of 4, then zeroed payload.
struct SpawnEventMessage {
    void* vtbl;
    int32_t field_04;
    int32_t field_08;
    int32_t field_0C;
    int32_t field_10;
    int32_t field_14;
    int32_t field_18;
    int32_t field_1C;
    int32_t field_20;
    int32_t field_24;
    int32_t field_28;
};

static int __stdcall SpawnPoints_FlushPendingSpawnRecords(int owner) {
    auto* self = reinterpret_cast<uint8_t*>(owner);
    int result = 0;

    while (true) {
        result = GetNextPendingSpawnRecord(self, nullptr);
        if (!result)
            break;

        int32_t* record = reinterpret_cast<int32_t*>(result);

        int valueIfNotFlagged = GetValueIfNotFlagged(self + 0x70, nullptr);
        if (valueIfNotFlagged == 0)
            continue;

        int entryAddress = valueIfNotFlagged - 0x14;
        if (entryAddress == 0)
            continue;

        uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);
        void* target = reinterpret_cast<void*>(CSF_AT(int, entry, 0x38));
        if (target == nullptr)
            continue;

        SpawnEventMessage message{};
        message.vtbl = CSF_VTBL(0x0082D448);
        message.field_04 = -1;
        message.field_08 = -1;
        message.field_0C = record[0];
        message.field_10 = record[1];
        message.field_14 = 4;
        message.field_18 = 0;
        message.field_1C = 0;
        message.field_20 = 0;
        message.field_24 = 0;
        message.field_28 = 0;

        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, target, 0)[140 / 4])(target, nullptr, &message);

        message.vtbl = CSF_VTBL(0x0082D410);
    }

    return result;
}

HOOK(0x005534D0, SpawnPoints_FlushPendingSpawnRecords, "auto");
