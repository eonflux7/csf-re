#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Scans the record table of a C_0083eb70 (12-byte records, key stored at record+4)
// for a record whose key equals the key at the second argument's +4 field.
// Returns 1 when found, 0 otherwise.
static char __fastcall C_0083eb70_HasRecordWithKey(C_0083eb70* self, void* /*edx*/, int a2) {
    const uint32_t recordCount = self->field_0098;
    char result = 0;
    if (recordCount == 0) {
        return result;
    }

    const uint32_t searchKey = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 0x4);
    uint8_t* recordKeyAddr = reinterpret_cast<uint8_t*>(self->field_0090) + 4;
    uint32_t recordIndex = 0;

    for (;;) {
        if (CSF_AT(uint32_t, recordKeyAddr, 0x0) == searchKey) {
            return 1;
        }
        ++recordIndex;
        if (recordIndex >= recordCount) {
            return result;
        }
        recordKeyAddr += 12;
    }
}

HOOK(0x4E73F0, C_0083eb70_HasRecordWithKey, "auto");
