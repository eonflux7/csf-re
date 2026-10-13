// Auto-generated reimplementation of S_0044bc20 lookup (0x0044BCF0).
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scans a table of 100-byte records (begin = field_0010, end = field_0014).
// Each record holds a Vec3 at offset 4. Returns the index of the first record
// whose Vec3 equals a2[0..2], or -1 if none matches.
static int __fastcall S_0044bc20_FindIndexByVec3(S_0044bc20* self, void* /*edx*/, float* a2)
{
    uint32_t begin = self->field_0010;

    int recordCount = 0;
    if (begin)
    {
        int32_t byteSpan = static_cast<int32_t>(self->field_0014 - begin);
        recordCount = byteSpan / 100;
    }

    if (recordCount == 0)
        return -1;

    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(begin));
    int index = 0;

    for (;;)
    {
        float recX = CSF_AT(float, record, 0x4);
        float recY = CSF_AT(float, record, 0x8);
        float recZ = CSF_AT(float, record, 0xC);

        if (recX == a2[0] && recY == a2[1] && recZ == a2[2])
            return index;

        if (++index >= recordCount)
            return -1;

        record += 100;
    }
}

HOOK(0x44BCF0, S_0044bc20_FindIndexByVec3, "auto");
