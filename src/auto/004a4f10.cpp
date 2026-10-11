#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scans a table of 8-byte records (key dword first) and returns the index of the
// first record whose key equals `key`, or -1 when the table is empty or no record matches.
static int __fastcall GetRecordIndexByKey(C_00834598* self, void* /*edx*/, int key)
{
    const uint32_t recordCount = self->field_0084;
    int result = -1;

    if (recordCount == 0)
        return result;

    int* record = reinterpret_cast<int*>(self->field_007c);
    uint32_t index = 0;
    for (;;)
    {
        if (*record == key)
            return static_cast<int>(index);

        ++index;
        if (index >= recordCount)
            return result;

        record += 2; // records are 8 bytes wide
    }
}

HOOK(0x004A4F10, GetRecordIndexByKey, "auto");
