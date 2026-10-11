#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sums, over every waypoint-graph record whose kind matches the argument, the
// number of entries in that record's list, computed as (end - begin) / 44.
// Returns 0 when there are no records.
static int __fastcall C_0083e0d8_CountPointsOfKind(C_0083e0d8* self, void* /*edx*/, int kind) {
    // Record count = (records_end - records_begin) / 72.
    int recordCount = 0;
    if (self->field_0008 != 0) {
        recordCount = static_cast<int>(self->field_000c - self->field_0008) / 72;
    }
    if (recordCount == 0) {
        return 0;
    }

    // Cursor starts at the first record's list-header field (record + 0x38) and
    // advances by 72 bytes per record, mirroring the original's walk.
    uint8_t* cursor = reinterpret_cast<uint8_t*>(self->field_0008) + 0x38;
    int remaining = recordCount;
    int totalPoints = 0;
    do {
        // Record kind lives 0x18 bytes before the cursor (record + 0x20).
        if (CSF_AT(uint32_t, cursor, -0x18) == static_cast<uint32_t>(kind)) {
            uint32_t listBegin = CSF_AT(uint32_t, cursor, 0);
            if (listBegin != 0) {
                uint32_t listEnd = CSF_AT(uint32_t, cursor, 4);
                totalPoints += static_cast<int>(listEnd - listBegin) / 44;
            }
        }
        cursor += 0x48;
        --remaining;
    } while (remaining != 0);

    return totalPoints;
}

HOOK(0x0075C520, C_0083e0d8_CountPointsOfKind, "auto");
