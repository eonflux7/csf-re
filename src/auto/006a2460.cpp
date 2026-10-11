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

// sub_6A20E0: __stdcall (void* a1, int a2), ret 8
CSF_FUNC(uint16_t*, __stdcall, ProcessRecordAtOffset, 0x006A20E0, (void* a1, int a2));

// Walks a record range [begin, end) in 16-byte steps and processes each record.
// Returns the result of the last ProcessRecordAtOffset call, or nullptr when the range is empty.
static uint16_t* __stdcall ProcessRecordRange(S_00694cd0* a1, int a2)
{
    auto* range = reinterpret_cast<uint8_t*>(a2);

    uint16_t* result = nullptr;
    uint32_t recordCount = 0;

    uint32_t begin = CSF_AT(uint32_t, range, 4);
    if (begin != 0)
    {
        // Signed shift, as in the original (sar).
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, range, 8) - begin);
        recordCount = static_cast<uint32_t>(byteSpan >> 4);
    }
    result = reinterpret_cast<uint16_t*>(static_cast<uintptr_t>(recordCount));

    if (recordCount != 0)
    {
        uint32_t offset = 0;
        uint32_t remaining = recordCount;
        do
        {
            int entryOffset = static_cast<int>(CSF_AT(uint32_t, range, 4) + offset);
            result = ProcessRecordAtOffset(a1, entryOffset);
            offset += 16;
            remaining = remaining - 1;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x006A2460, ProcessRecordRange, "auto");
