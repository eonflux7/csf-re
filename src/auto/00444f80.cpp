#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall(this = list header, a2 = search record). Returns the index of the first
// 12-byte entry whose dword at +4 equals the search record's dword at +4, or -1.
static int __fastcall FindEventRecordIndex(void* self, void* /*edx*/, void* searchRecord)
{
    const uint32_t recordCount = CSF_AT(uint32_t, self, 8);
    int resultIndex = 0;

    if (recordCount == 0)
        return -1;

    uint8_t* recordPtr = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0)) + 4;
    const uint32_t wantedKey = CSF_AT(uint32_t, searchRecord, 4);

    for (;;)
    {
        if (CSF_AT(uint32_t, recordPtr, 0) == wantedKey)
            return resultIndex;

        ++resultIndex;
        if (static_cast<uint32_t>(resultIndex) >= recordCount)
            return -1;

        recordPtr += 12;
    }
}

HOOK(0x444F80, FindEventRecordIndex, "auto");
