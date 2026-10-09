#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the address of the first record's inline name buffer (+4) when its
// capacity field (+0x18) is below 16, otherwise the heap pointer stored at +4.
// Returns 0 when the container is empty.
static int __fastcall S_00500350_GetFirstRecordNameBuffer(S_00500350* self, void* /*edx*/) {
    uint32_t firstRecordAddr = self->field_0030;
    if (firstRecordAddr == 0) {
        return 0;
    }

    // Signed element count: (end - begin) / sizeof(record), record size 88 bytes.
    int recordCount = static_cast<int>(self->field_0034 - firstRecordAddr) / 88;
    if (recordCount == 0) {
        return 0;
    }

    uint8_t* firstRecord = reinterpret_cast<uint8_t*>(self->field_0030);
    if (CSF_AT(uint32_t, firstRecord, 0x18) < 0x10u) {
        return static_cast<int>(self->field_0030 + 4);
    }
    return static_cast<int>(CSF_AT(uint32_t, firstRecord, 4));
}

HOOK(0x500350, S_00500350_GetFirstRecordNameBuffer, "auto");
