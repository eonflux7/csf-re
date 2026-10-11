#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, RenderQueueGrowRecords, 0x00446340,
         (void* self, void* edx, int a2));

static uint32_t* __fastcall RenderQueuePushRecord(
    void* self, void* /*edx*/, uint32_t* record)
{
    (void)CSF_VTBL(0x00446340);

    const uint32_t count = CSF_AT(uint32_t, self, 0x14);
    uint32_t index = 0;
    uint32_t* destination = nullptr;

    if (count != 0) {
        uint32_t* const records = CSF_AT(uint32_t*, self, 0x0C);
        uint32_t* candidate = records;
        while (index < count) {
            if (candidate[0] == UINT32_MAX) {
                if (index != UINT32_MAX)
                    destination = records + 9 * index;
                break;
            }
            ++index;
            candidate += 9;
        }
    }

    if (destination == nullptr) {
        void* const storage = static_cast<uint8_t*>(self) + 0x0C;
        if (CSF_AT(uint32_t, self, 0x14) >=
            CSF_AT(uint32_t, self, 0x10)) {
            RenderQueueGrowRecords(
                storage, nullptr, CSF_AT(int, self, 0x18));
        }

        const uint32_t nextIndex = CSF_AT(uint32_t, self, 0x14);
        destination = CSF_AT(uint32_t*, self, 0x0C) + 9 * nextIndex;
        CSF_AT(uint32_t, self, 0x14) = nextIndex + 1;
    }

    destination[0] = record[0];
    destination[2] = record[2];
    destination[3] = record[3];
    destination[4] = record[4];
    destination[5] = record[5];
    destination[6] = record[6];
    destination[7] = record[7];
    destination[8] = record[8];
    return destination;
}

HOOK(0x00446D30, RenderQueuePushRecord, "auto");
