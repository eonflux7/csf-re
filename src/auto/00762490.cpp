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

// Searches the character/vehicle record table (reached through the pointer at +0x8)
// for the record whose leading ID dword equals recordId.
// Records are 556 (0x22C) bytes each, stored in the range [begin, end) at table+4 / table+8.
// Returns the record index, or -1 if the table is empty or the ID is not present.
static int __fastcall C_0086f128_FindRecordIndexById(C_0086f128* self, void* /*edx*/, int recordId)
{
    uint8_t* tableHeader = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x8)));
    uint32_t firstRecordAddr = CSF_AT(uint32_t, tableHeader, 0x4);

    int32_t recordCountSigned = 0;
    if (firstRecordAddr != 0)
    {
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, tableHeader, 0x8) - firstRecordAddr);
        recordCountSigned = byteSpan / 556;
    }
    uint32_t recordCount = static_cast<uint32_t>(recordCountSigned);

    if (recordCount == 0)
        return -1;

    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstRecordAddr));
    for (uint32_t index = 0; index < recordCount; ++index)
    {
        if (CSF_AT(int, record, 0) == recordId)
            return static_cast<int>(index);
        record += 0x22C;
    }
    return -1;
}

HOOK(0x762490, C_0086f128_FindRecordIndexById, "auto");
