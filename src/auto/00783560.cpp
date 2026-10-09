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

// Finds the index of the weapon record whose leading ID dword equals recordId.
// Records are 552 (0x228) bytes each, stored in a range [begin, end) held by the
// inner table object pointed to by the m_pRecords slot. Returns -1 if not found.
static int __fastcall CWeaponDefTable_FindRecordIndexById(CWeaponDefTable* self, void* /*edx*/, int recordId)
{
    uint8_t* recordRange = reinterpret_cast<uint8_t*>(self->m_pRecords);
    uint32_t firstRecordAddr = CSF_AT(uint32_t, recordRange, 0x4);

    int recordCount = 0;
    if (firstRecordAddr != 0)
    {
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, recordRange, 0x8) - firstRecordAddr);
        recordCount = byteSpan / 552;
    }

    if (recordCount == 0)
        return -1;

    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstRecordAddr));
    for (int index = 0; index < recordCount; ++index)
    {
        if (CSF_AT(int, record, 0) == recordId)
            return index;
        record += 0x228;
    }
    return -1;
}

HOOK(0x783560, CWeaponDefTable_FindRecordIndexById, "auto");
