#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Searches a contiguous array of 100-byte records (begin at field_0010, end at field_0014)
// for a record whose dword at +0x38 equals `key`. Returns the record index, or -1.
static int __fastcall S_0044bc20_FindIndexByKey(S_0044bc20* self, void* /*edx*/, int key)
{
    unsigned int recordCount = 0;
    if (self->field_0010) {
        const int byteSpan = static_cast<int>(self->field_0014 - self->field_0010);
        recordCount = static_cast<unsigned int>(byteSpan / 100);
    }

    int index = 0;
    if (recordCount == 0) {
        return -1;
    }

    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0010));
    while (CSF_AT(int, record, 0x38) != key) {
        if (static_cast<unsigned int>(++index) >= recordCount) {
            return -1;
        }
        record += 100;
    }
    return index;
}

HOOK(0x44BC20, S_0044bc20_FindIndexByKey, "auto");
