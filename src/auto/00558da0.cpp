#include <cstdint>
#include "globals.h"
#include "hook.h"

// Mem_FreeDebug at 0x690E60 (game heap free used by the original).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Walks 20-byte records from firstRecord up to (not including) endRecord.
// For each record, frees the pointer stored at +8 (if non-null) and zeroes
// the three dwords at +8, +12, +16. Returns endRecord.
static int __cdecl MpRules_FreeAndClearRecordRange(int firstRecord, int endRecord)
{
    int cursor = firstRecord;
    if (firstRecord != endRecord)
    {
        do
        {
            uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(cursor));
            if (CSF_AT(uint32_t, record, 0x08) != 0)
                Mem_FreeDebug(CSF_AT(void*, record, 0x08));
            CSF_AT(uint32_t, record, 0x08) = 0;
            CSF_AT(uint32_t, record, 0x0C) = 0;
            CSF_AT(uint32_t, record, 0x10) = 0;
            cursor += 20;
        } while (cursor != endRecord);
    }
    return endRecord;
}

HOOK(0x558DA0, MpRules_FreeAndClearRecordRange, "auto");
